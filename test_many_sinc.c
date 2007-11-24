/*
   Part of: annealing
   Contents: many-tries annealing test with sinc(t) minimisation
   Date: Mon Feb 26, 2007
   
   Abstract
   
	Find the minimum of 'f(t) = -sin(t)/t'.
   
   Copyright (c) 2007 Marco Maggi
   
   This is free  software you can redistribute it  and/or modify it under
   the terms of  the GNU General Public License as  published by the Free
   Software Foundation; either  version 2, or (at your  option) any later
   version.
   
   This  file is  distributed in  the hope  that it  will be  useful, but
   WITHOUT   ANY  WARRANTY;  without   even  the   implied  warranty   of
   MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
   General Public License for more details.
   
   You  should have received  a copy  of the  GNU General  Public License
   along with this file; see the file COPYING.  If not, write to the Free
   Software Foundation,  Inc., 59  Temple Place -  Suite 330,  Boston, MA
   02111-1307, USA.
*/


/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <float.h>
#include <gsl/gsl_rng.h>
#include "gsl_annealing.h"


static	gsl_annealing_energy_fun_t	energy_function;
static	gsl_annealing_step_fun_t	step_function;
static	gsl_annealing_log_fun_t		log_function;
static	gsl_annealing_copy_fun_t	copy_function;


/* Why do I have to do this even if I have included "unistd.h"? */
extern int getopt (int argc, char ** argv, const char * options);



/** ------------------------------------------------------------
 ** Main.
 ** ----------------------------------------------------------*/

#define TRIES	10

int
main (int argc, char ** argv)
{
  gsl_annealing_manytries_workspace_t	S;
  gsl_annealing_configuration_t		array[TRIES];
  double	configurations[2 + TRIES]; /* best, current and new tries */
  double	monte_carlo_coordinates[TRIES];
  double	max_step = 10.0;
  int		verbose_mode = 0;
  

  {
    int c;

    while ((c = getopt(argc, argv, "hv")) != -1)
      switch (c)
	{
	case 'h':
	  fprintf(stderr, "usage: test_many_sinc [-v] [-h]\n");
	  exit(EXIT_SUCCESS);
	case 'v':
	  verbose_mode = 1;
	  break;
	default:
	  fprintf(stderr, "test_many_sinc error: unknown option %c\n", c);
	  exit(EXIT_FAILURE);
	}
  }

  printf("\n------------------------------------------------------------\n");
  printf("test_many_sinc: many-tries sinc minimisation with simulated annealing\n");

  S.number_of_iterations_at_fixed_temperature = 10;
  S.max_step_value		= &max_step;

  S.temperature			= 10.0;
  S.minimum_temperature		= 1.0e-6;
  S.restart_temperature		= DBL_MIN; /* do not restart */
  S.boltzmann_constant		= 1.0;
  S.damping_factor		= 1.005;

  S.energy_function		= energy_function;
  S.step_function		= step_function;
  S.copy_function		= copy_function;
  S.log_function		= (verbose_mode)? log_function : NULL;

  S.numbers_generator		= gsl_rng_alloc(gsl_rng_rand);
  gsl_rng_set(S.numbers_generator, 15);

  S.current_configuration.data	= &(configurations[0]);
  S.best_configuration.data	= &(configurations[1]);
  S.new_configurations		= array;
  S.monte_carlo_coordinates	= monte_carlo_coordinates;
  S.number_of_tries		= TRIES;

  for (size_t i=0; i<S.number_of_tries; ++i)
    array[i].data = &(configurations[i+2]);

  configurations[0] = 100.0;

  gsl_annealing_manytries_solve(&S);

  printf("test_many_sinc: final best solution: %f, global 0.0\n", configurations[1]);
  printf("------------------------------------------------------------\n\n");

  gsl_rng_free(S.numbers_generator);
  exit(EXIT_SUCCESS);
}


/** ------------------------------------------------------------
 ** Iteration functions.
 ** ----------------------------------------------------------*/

static double
alea (gsl_annealing_manytries_workspace_t * S)
{
  double	max_step = *((double *)S->max_step_value);

  return (2.0 * gsl_rng_uniform(S->numbers_generator) - 1.0) * max_step;
}

/* ------------------------------------------------------------ */

double
energy_function (void * dummy, void * configuration)
{
  double	C = *((double *)configuration);

  return -sin(C)/C;
}
void
step_function (void * W, void * configuration)
{
  gsl_annealing_manytries_workspace_t * S = W;
  double *	C = (double *)configuration;
  double	c;

  do c = *C + alea(S); while (fabs(c) > 120.0);
  *C = c;
}
void
log_function (void * W)
{
  gsl_annealing_manytries_workspace_t * S = W;
  double *	current = (double *)S->current_configuration.data;
  double *	best    = (double *)S->best_configuration.data;

  printf("current %f (energy %f), best %f (energy %f)\n",
	 *current, S->current_configuration.energy,
	 *best,    S->best_configuration.energy);
}


/** ------------------------------------------------------------
 ** Configuration handling functions.
 ** ----------------------------------------------------------*/

void
copy_function (void * dummy, void * dst_configuration, void * src_configuration)
{
  double *	dst = dst_configuration;
  double *	src = src_configuration;

  *dst = *src;
}


/* end of file */
