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
#include <float.h>
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
  assert(S->number_of_iterations_at_fixed_temperature > 0);
  assert(S->boltzmann_constant > 0.0);
  assert(S->temperature > 0.0 && S->minimum_temperature > 0.0);
  assert(S->temperature > S->restart_temperature);
  assert(S->damping_factor > 1.0);
  assert(S->copy_function && S->energy_function && S->step_function);
  assert(S->numbers_generator);

  S->copy_function(S, S->best_configuration, S->configuration);
  S->copy_function(S, S->new_configuration, S->configuration);
  S->best_energy = S->new_energy = S->energy =
    S->energy_function(S, S->configuration);

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
	  S->restart_temperature = DBL_MIN;
	}
    }
  /* The result is in 'S->best_configuration'. */
}


/** ------------------------------------------------------------
 ** Multi-best algorithm.
 ** ----------------------------------------------------------*/

static int
find_best_configuration_position (gsl_annealing_multi_workspace_t * S)
{
  int	position;

  for (position=S->best_configurations_count-1; position>=0; --position)
    {
      if (S->new_energy > S->best_energies[position])
	break;
    }
  for (int i=0; i<position; ++i)
    {
      double	distance =
	S->metric_function(S, S->best_configurations[i], S->new_configuration);

      if (distance < S->minimum_acceptance_distance) return -1;
    }
  return position;
}
static void
shift_best_configurations (gsl_annealing_multi_workspace_t * S, int idx)
{
  /*
    Example: before:

	best_configurations_max   = 5
	best_configurations_count = 3
	idx = 1

	best_configurations -> | c0 | c1 | c2 | empty | empty |
    
    after:

	best_configurations_max   = 5
	best_configurations_count = 4

	best_configurations -> | c0 | empty | c1 | c2 | empty |

  */
  for (int i=S->best_configurations_count-1; i>=idx; --i)
    {
      S->copy_function(S, S->best_configurations[i+1], S->best_configurations[i]);
      S->best_energies[i+1] = S->best_energies[i];
    }
}
static void
store_new_at_position (gsl_annealing_multi_workspace_t * S, int idx)
{
  S->copy_function(S, (void *)S->best_configurations[idx], S->new_configuration);
  S->best_energies[idx] = S->new_energy;
}

/* ------------------------------------------------------------ */

void 
gsl_annealing_multi_solve (gsl_annealing_multi_workspace_t * S)
{
  double	best_energies_array[S->best_configurations_max];

  assert(S->number_of_iterations_at_fixed_temperature > 0);
  assert(S->boltzmann_constant > 0.0);
  assert(S->temperature > 0.0 && S->minimum_temperature > 0.0);
  assert(S->damping_factor > 1.0);
  assert(S->copy_function && S->energy_function && S->step_function);
  assert(S->numbers_generator);

  S->best_energies = best_energies_array;

  S->copy_function(S, (void *)S->best_configurations[0], S->configuration);
  S->copy_function(S, S->new_configuration, S->configuration);
  S->best_energies[0] = S->new_energy = S->energy =
    S->energy_function(S, S->configuration);

  S->best_configurations_count = 1;
  if (S->log_function) S->log_function(S);
  
  for (;;)
    {
      for (size_t i=0; i < S->number_of_iterations_at_fixed_temperature; ++i)
	{
	  S->copy_function(S, S->new_configuration, S->configuration);
	  S->step_function(S, S->new_configuration);
	  S->new_energy = S->energy_function(S, S->new_configuration);

	  if (S->new_energy <= S->best_energies[S->best_configurations_count-1])
	    {
	      int	position;

	      position = find_best_configuration_position(S);
	      if (0 < position)
		{
		  shift_best_configurations(S, position);
		  store_new_at_position(S, position);
		}
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
    }
  /* The results are in 'S->best_configurations'. */
}


/* end of file */
