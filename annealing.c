/* annealing.c
 * 
 * Copyright (C) 1996, 1997, 1998, 1999, 2000 Mark Galassi
 * Copyright (C) 2007 Marco Maggi
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <config.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <gsl/gsl_machine.h>
#include <gsl/gsl_rng.h>
#include "gsl_annealing.h"



/** ------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------*/

/* Avoid underflow errors for large uphill steps. */
static inline 
double safe_exp (double x)
{ 
  return ((x < GSL_LOG_DBL_MIN) ? 0.0 : exp(x));
}

/* ------------------------------------------------------------ */

#define random_level(S)		gsl_rng_uniform(S->numbers_generator)
#define new_level(S)		\
	safe_exp(-(S->new_energy - S->energy)/(S->boltzmann_constant * S->temperature))

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Simple algorithm.
 ** ----------------------------------------------------------*/

void 
gsl_annealing_simple_solve (gsl_annealing_simple_workspace_t * S)
{
  S->copy_function(S, S->best_configuration, S->configuration);
  S->best_energy = S->energy = S->energy_function(S, S->configuration);
  if (S->log_function) S->log_function(S);

  for (;;)
    {
      for (size_t i=0; i < S->number_of_iterations_at_fixed_temperature; ++i)
	{
	  S->copy_function(S, S->new_configuration, S->configuration);
	  S->step_function(S, S->new_configuration);
	  S->new_energy = S->energy_function(S, S->new_configuration);

	  if (S->new_energy <= S->best_energy)
	    {
	      S->copy_function(S, S->best_configuration, S->new_configuration);
	      S->copy_function(S, S->configuration,      S->new_configuration);
	      S->energy = S->best_energy = S->new_energy;
	    }
	  else if (random_level(S) < new_level(S))
	    {
	      S->copy_function(S, S->configuration, S->new_configuration);
	      S->energy = S->new_energy;
	    }
	}
      if (S->log_function) S->log_function(S);

      S->temperature /= S->damping_factor;
      if (S->temperature < S->minimum_temperature) break;
      if (S->temperature < S->restart_temperature)
	{
	  S->copy_function(S, S->configuration, S->best_configuration);
	  S->energy = S->best_energy;
	}
    }
  /* The result is in 'S->best_configuration'. */
}


/** ------------------------------------------------------------
 ** Many tries algorithm.
 ** ----------------------------------------------------------*/

#if 0
void 
gsl_siman_solve_many (const gsl_rng * r, void *x0_p, gsl_siman_Efunc_t Ef,
                      gsl_siman_step_t take_step,
                      gsl_siman_metric_t distance,
                      gsl_siman_print_t print_position,
                      size_t element_size,
                      gsl_siman_params_t params)
{
  /* the new set of trial points, and their energies and probabilities */
  void *x, *new_x;
  double *energies, *probs, *sum_probs;
  double Ex;                    /* energy of the chosen point */
  double T;                     /* the temperature */
  int i, done;
  double u;                     /* throw the die to choose a new "x" */
  int n_iter;

  if (print_position) {
    printf ("#-iter    temperature       position");
    printf ("         delta_pos        energy\n");
  }

  x = (void *) malloc (params.n_tries * element_size);
  new_x = (void *) malloc (params.n_tries * element_size);
  energies = (double *) malloc (params.n_tries * sizeof (double));
  probs = (double *) malloc (params.n_tries * sizeof (double));
  sum_probs = (double *) malloc (params.n_tries * sizeof (double));

  T = params.t_initial;
/*    memcpy (x, x0_p, element_size); */
  memcpy (x, x0_p, element_size);
  done = 0;

  n_iter = 0;
  while (!done)
    {
      Ex = Ef (x);
      for (i = 0; i < params.n_tries - 1; ++i)
        {                       /* only go to N_TRIES-2 */
          /* center the new_x[] around x, then pass it to take_step() */
          sum_probs[i] = 0;
          memcpy ((char *)new_x + i * element_size, x, element_size);
          take_step (r, (char *)new_x + i * element_size, params.step_size);
          energies[i] = Ef ((char *)new_x + i * element_size);
          probs[i] = safe_exp (-(energies[i] - Ex) / (params.k * T));
        }
      /* now add in the old value of "x", so it is a contendor */
      memcpy ((char *)new_x + (params.n_tries - 1) * element_size, x, element_size);
      energies[params.n_tries - 1] = Ex;
      probs[params.n_tries - 1] = safe_exp (-(energies[i] - Ex) / (params.k * T));

      /* now throw biased die to see which new_x[i] we choose */
      sum_probs[0] = probs[0];
      for (i = 1; i < params.n_tries; ++i)
        {
          sum_probs[i] = sum_probs[i - 1] + probs[i];
        }
      u = gsl_rng_uniform (r) * sum_probs[params.n_tries - 1];
      for (i = 0; i < params.n_tries; ++i)
        {
          if (u < sum_probs[i])
            {
              memcpy (x, (char *)new_x + i * element_size, element_size);
              break;
            }
        }
      if (print_position)
        {
          printf ("%5d\t%12g\t", n_iter, T);
          print_position (x);
          printf ("\t%12g\t%12g\n", distance (x, x0_p), Ex);
        }
      T /= params.mu_t;
      ++n_iter;
      if (T < params.t_min)
        {
          done = 1;
        }
    }

  /* now return the value via x0_p */
  memcpy (x0_p, x, element_size);

  /*  printf("the result is: %g (E=%g)\n", x, Ex); */

  free (x);
  free (new_x);
  free (energies);
  free (probs);
  free (sum_probs);
}
#endif


/* end of file */
