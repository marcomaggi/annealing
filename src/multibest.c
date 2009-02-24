/* multibest.c
 *
 * Copyright (C) 2007, 2009 Marco Maggi <marcomaggi@gna.org>
 * Copyright (C) 1996, 1997, 1998, 1999, 2000 Mark Galassi
 *
 * This program is free software:  you can redistribute it and/or modify
 * it under the terms of the  GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is  distributed in the hope that it  will be useful, but
 * WITHOUT  ANY   WARRANTY;  without   even  the  implied   warranty  of
 * MERCHANTABILITY  or FITNESS FOR  A PARTICULAR  PURPOSE.  See  the GNU
 * General Public License for more details.
 *
 * You should  have received  a copy of  the GNU General  Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */



/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#define current_configuration		current
#define new_configuration		new
#define best_configurations		bests
#include "internal.h"


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
#define accept_new_as_best(S)		copy_configuration(S, bests[0],	new)
#define set_current_to_best(S)		copy_configuration(S, current,	bests[0])
#define set_best_to_current(S)		copy_configuration(S, bests[0],	current)
#define set_new_to_current(S)		copy_configuration(S, new,	current)


/** ------------------------------------------------------------
 ** Insert sort functions.
 ** ----------------------------------------------------------*/

static int
find_best_configuration_position (annealing_multibest_workspace_t * S)
{
  int	position;

  assert(S->new.energy <= S->best_configurations[S->best_configurations_count-1].energy);
  for (position=0; position<(int)S->best_configurations_count; ++position)
    {
      if (S->new.energy < S->bests[position].energy)
	break;
    }
  for (int i=0; i<=position; ++i)
    {
      double	distance = S->metric_function(S, S->bests[i].data, S->new.data);

      if (distance < S->minimum_acceptance_distance) return -1;
    }
  return position;
}
static void
insert_new_at_best_configurations_position (annealing_multibest_workspace_t * S,
					    int position)
{
  int	last;

  /*
    Example: before:

	max_number_of_best_configurations = 5
	best_configurations_count         = 3
	position = 1

	bests -> | c0 | c1 | c2 | empty | empty |

    after:

	max_number_of_best_configurations = 5
	best_configurations_count         = 4

	bests -> | c0 | empty | c1 | c2 | empty |

  */
  last = (S->best_configurations_count == S->max_number_of_best_configurations)?
    (S->best_configurations_count-2) : (S->best_configurations_count-1);

  for (int i=last; i>=position; --i)
    {
      S->copy_function(S, S->bests[i+1].data, S->bests[i].data);
      S->bests[i+1].energy = S->bests[i].energy;
    }
  if (S->best_configurations_count < S->max_number_of_best_configurations)
    ++(S->best_configurations_count);
  S->copy_function(S, S->bests[position].data, S->new.data);
  S->bests[position].energy = S->new.energy;
/*   printf("accepted %.5g (E = %.5f), in position %d; stack:", */
/* 	 *((double *)(S->bests[position].data)), S->bests[position].energy, */
/* 	 position); */
/*   for (size_t i=0; i<S->best_configurations_count; ++i) */
/*     printf(" %5g", *((double *)(S->bests[i].data))); */
/*   printf("\n"); */
}


/** ------------------------------------------------------------
 ** Multi-best algorithm.
 ** ----------------------------------------------------------*/

void
annealing_multibest_solve (annealing_multibest_workspace_t * S)
{
  assert(S->number_of_iterations_at_fixed_temperature > 0);
  assert(S->boltzmann_constant > 0.0);
  assert(S->temperature > 0.0 && S->minimum_temperature > 0.0);
  assert(S->damping_factor > 1.0);
  assert(S->copy_function && S->energy_function &&
	 S->step_function && S->metric_function);
  assert(S->numbers_generator);


  compute_current_energy(S);
  set_best_to_current(S);
  set_new_to_current(S);
  S->best_configurations_count = 1;
  if (S->log_function) S->log_function(S);

  for (;;)
    {
      for (size_t i=0; i < S->number_of_iterations_at_fixed_temperature; ++i)
	{
	  take_step(S);
	  if (S->new.energy <= S->bests[S->best_configurations_count-1].energy)
	    {
	      int	position;

	      position = find_best_configuration_position(S);
	      if (0 <= position)
		{
		  insert_new_at_best_configurations_position(S, position);
		}
	      accept_new_as_current(S);
	    }
	  else if ((S->new.energy <= S->current.energy) ||
		   (random_level(S) < new_level(S)))
	    {
	      accept_new_as_current(S);
	    }
	}
      if (S->log_function) S->log_function(S);

      if (S->cooling_function)
	S->cooling_function(S);
      else
	S->temperature /= S->damping_factor;

      if (S->temperature < S->minimum_temperature) break;
    }
  /* The results are in 'S->best_configurations'. */
}


/* end of file */
