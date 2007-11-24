/* basic_annealing.c
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



/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#include <config.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <assert.h>

#include <gsl/gsl_machine.h>
#include <gsl/gsl_rng.h>

#define current_configuration		current
#define new_configuration		new
#define best_configuration		best
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
#define new_level(S)						\
 safe_exp(-(S->new.energy-S->current.energy)/(S->boltzmann_constant*S->temperature))

#define compute_current_energy(S)			\
    S->current.energy = S->energy_function(S, S->current.data)

#define compute_new_energy(S)			\
    S->new.energy = S->energy_function(S, S->new.data)

#define take_step(S)					\
  {							\
    S->copy_function(S, S->new.data, S->current.data);	\
    S->step_function(S, S->new.data);			\
    compute_new_energy(S);				\
  }

/* ------------------------------------------------------------ */

#define copy_configuration(S,dst,src)			\
  {							\
    S->copy_function(S, S->dst.data, S->src.data);	\
    S->dst.energy = S->src.energy;			\
  }

#define accept_new_as_current(S)	copy_configuration(S, current,	new)
#define accept_new_as_best(S)		copy_configuration(S, best,	new)
#define set_current_to_best(S)		copy_configuration(S, current,	best)
#define set_best_to_current(S)		copy_configuration(S, best,	current)
#define set_new_to_current(S)		copy_configuration(S, new,	current)

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Simple algorithm.
 ** ----------------------------------------------------------*/

void 
gsl_annealing_simple_solve (gsl_annealing_simple_workspace_t * S)
{
  assert(S->number_of_iterations_at_fixed_temperature > 0);
  assert(S->boltzmann_constant > 0.0);
  assert(S->temperature > 0.0 && S->minimum_temperature > 0.0);
  assert(S->temperature > S->restart_temperature);
  assert(S->damping_factor > 1.0);
  assert(S->copy_function && S->energy_function && S->step_function);
  assert(S->numbers_generator);

  S->restart_flag = 0;
  compute_current_energy(S);
  set_best_to_current(S);
  set_new_to_current(S);

  if (S->log_function) S->log_function(S);

  for (;;)
    {
      for (size_t i=0; i < S->number_of_iterations_at_fixed_temperature; ++i)
	{
	  take_step(S);
	  if (S->new.energy <= S->best.energy)
	    {
	      accept_new_as_best(S);
	      accept_new_as_current(S);
	    }
	  else if ((S->new.energy <= S->current.energy) ||
		   (random_level(S) < new_level(S)))
	    {
	      accept_new_as_current(S);
	    }
	}
      if (S->log_function) S->log_function(S);

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
