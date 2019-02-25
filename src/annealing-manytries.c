/* annealing-manytries.c
 *
 * Copyright (C) 2007, 2009, 2019 Marco Maggi <marcomaggi@gna.org>
 * Copyright (C) 1996, 1997, 1998, 1999, 2000 Mark Galassi
 *
 * This program is free software: you can  redistribute it and/or modify it under the
 * terms  of  the GNU  General  Public  License as  published  by  the Free  Software
 * Foundation,  either version  3  of the  License,  or (at  your  option) any  later
 * version.
 *
 * This program is  distributed in the hope  that it will be useful,  but WITHOUT ANY
 * WARRANTY; without  even the implied warranty  of MERCHANTABILITY or FITNESS  FOR A
 * PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of  the GNU General Public License along with this
 * program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */



/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#define current_configuration		current
#define new_configurations		news
#define best_configuration		best
#include "annealing-internals.h"


/** ------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------*/

/* Avoid underflow errors for large uphill steps. (GSL_LOG_DBL_MIN=-7.08...e2) */
static inline
double safe_exp (double x)
{
  return ((x < GSL_LOG_DBL_MIN) ? 0.0 : exp(x));
}

static double
compute_fraction (annealing_manytries_workspace_t * S, double energy)
{
  double R = -(energy - S->current.energy)/(S->boltzmann_constant * S->temperature);

  return (R < 10.0)? safe_exp(R) : 10.0;
}

#define compute_current_energy(S)			\
    S->current.energy = S->energy_function(S, S->current.data)

/* ------------------------------------------------------------ */

#define copy_configuration(S,dst,src)			\
  {							\
    S->copy_function(S, S->dst.data, S->src.data);	\
    S->dst.energy = S->src.energy;			\
  }

#define accept_new_as_current(S,index)	copy_configuration(S, current,	news[index])
#define accept_new_as_best(S,index)	copy_configuration(S, best,	news[index])
#define set_current_to_best(S)		copy_configuration(S, current,	best)
#define set_best_to_current(S)		copy_configuration(S, best,	current)


/** ------------------------------------------------------------
 ** Many-tries algorithm.
 ** ----------------------------------------------------------*/

void
annealing_manytries_solve (annealing_manytries_workspace_t * S)
{
  double	x;
  size_t	selected_new;


  assert(S->number_of_tries > 0);
  assert(S->boltzmann_constant > 0.0);
  assert(S->temperature > 0.0 && S->minimum_temperature > 0.0);
  assert(S->temperature > S->restart_temperature);
  assert(S->damping_factor > 1.0);
  assert(S->copy_function && S->energy_function && S->step_function);
  assert(S->news);
  assert(S->monte_carlo_coordinates);
  assert(S->numbers_generator);

  compute_current_energy(S);
  set_best_to_current(S);
  S->restart_flag = 0;

  if (S->log_function) S->log_function(S);


  for (;;)
    {
      for (size_t i=0; i<S->number_of_tries; ++i)
	{
	  S->copy_function(S, S->news[i].data, S->current.data);
	  S->step_function(S, S->news[i].data);
	  S->news[i].energy = S->energy_function(S, S->news[i].data);

	  S->monte_carlo_coordinates[i] =
	    ((0 < i)? S->monte_carlo_coordinates[i-1] : 1.0) +
	    compute_fraction(S, S->news[i].energy);

	  if (S->news[i].energy < S->best.energy)
	    accept_new_as_best(S, i);
	}

/*       printf("tries E (N):"); */
/*       for (size_t i=0; i<S->number_of_tries; ++i) */
/* 	printf(" %.7f (%f)", S->news[i].energy, compute_fraction(S, S->news[i].energy)); */
/*       printf("\n"); */

      x = gsl_rng_uniform(S->numbers_generator) *
	S->monte_carlo_coordinates[S->number_of_tries-1];

/*       printf("monte carlo coordinates:"); */
/*       for (size_t i=0; i<S->number_of_tries; ++i) */
/* 	printf(" %.7f", S->monte_carlo_coordinates[i]); */
/*       printf("; random %f\n", x); */

      /* If 'x <= 1.0' we stay in the current configuration. */
      if (x > 1.0)
	{
	  for (selected_new=0; selected_new < S->number_of_tries; ++selected_new)
	    if (x > S->monte_carlo_coordinates[selected_new])
	      break;
	  accept_new_as_current(S, selected_new);
	}

      if (S->log_function) S->log_function(S);

      if (S->cooling_function)
	S->cooling_function(S);
      else
	S->temperature /= S->damping_factor;

      if (S->temperature < S->minimum_temperature) break;
      if (S->temperature < S->restart_temperature)
	{
	  set_current_to_best(S);
	  S->restart_temperature = DBL_MIN;
	  S->restart_flag = 1;
	}
    }
  /* The result is in 'S->best'. */
}

/* end of file */
