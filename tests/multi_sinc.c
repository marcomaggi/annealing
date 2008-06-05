/*
   Part of: annealing
   Contents: multibest annealing test with sinc(t) minimisation
   Date: Mon Feb 26, 2007
   
   Abstract
   
	Find the 4 local minima of 'f(t) = -sin(t)/t'.
   
   Copyright (c) 2007 Marco Maggi
   
   
   This  program  is free  software:  you  can redistribute  it
   and/or modify it  under the terms of the  GNU General Public
   License as published by the Free Software Foundation, either
   version  3 of  the License,  or (at  your option)  any later
   version.
   
   This  program is  distributed in  the hope  that it  will be
   useful, but  WITHOUT ANY WARRANTY; without  even the implied
   warranty  of  MERCHANTABILITY or  FITNESS  FOR A  PARTICULAR
   PURPOSE.   See  the  GNU  General Public  License  for  more
   details.
   
   You should  have received a  copy of the GNU  General Public
   License   along   with    this   program.    If   not,   see
   <http://www.gnu.org/licenses/>.
   
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
#include "annealing.h"


static	annealing_energy_fun_t	energy_function;
static	annealing_step_fun_t	step_function;
static	annealing_metric_fun_t	metric_function;
static	annealing_log_fun_t		log_function;
static	annealing_copy_fun_t	copy_function;


/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Main.
 ** ----------------------------------------------------------*/

int
main (int argc, char ** argv)
{
  annealing_multibest_workspace_t	S;
  annealing_configuration_t		array[4];
  double	configurations[2+4]; /* new, current and 4 best */
  double	max_step = 10.0;
  int		verbose_mode = 0;
  

  {
    int c;

    while ((c = getopt(argc, argv, "hv")) != -1)
      switch (c)
	{
	case 'h':
	  fprintf(stderr, "usage: test_multi_sinc [-v] [-h]\n");
	  exit(EXIT_SUCCESS);
	case 'v':
	  verbose_mode = 1;
	  break;
	default:
	  fprintf(stderr, "test_multi_sinc error: unknown option %c\n", c);
	  exit(EXIT_FAILURE);
	}
  }

  printf("\n------------------------------------------------------------\n");
  printf("test_multi_sinc: multi-best sinc minimisation with simulated annealing\n");

  S.number_of_iterations_at_fixed_temperature = 10;
  S.max_step_value		= &max_step;
  S.minimum_acceptance_distance	= 2.0;

  S.temperature			= 10.0;
  S.minimum_temperature		= 1.0e-6;
  S.boltzmann_constant		= 1.0;
  S.damping_factor		= 1.005;

  S.energy_function		= energy_function;
  S.step_function		= step_function;
  S.copy_function		= copy_function;
  S.log_function		= (verbose_mode)? log_function : NULL;
  S.metric_function		= metric_function;
  S.cooling_function		= NULL;

  S.numbers_generator		= gsl_rng_alloc(gsl_rng_rand);
  gsl_rng_set(S.numbers_generator, 15);

  S.max_number_of_best_configurations = 4;
  S.current_configuration.data	= &(configurations[0]);
  S.new_configuration.data	= &(configurations[1]);
  S.best_configurations		= array;

  array[0].data		= &(configurations[2]);
  array[1].data		= &(configurations[3]);
  array[2].data		= &(configurations[4]);
  array[3].data		= &(configurations[5]);
  configurations[0] = 100.0;

  annealing_multibest_solve(&S);

  printf("test_multi_sinc: found %u best solutions:",
	 S.best_configurations_count);
  for (size_t i=0; i<S.best_configurations_count; ++i)
    {
      double *	value = array[i].data;
      printf(" %.5f", *value);
    }
  printf("\n------------------------------------------------------------\n\n");

  gsl_rng_free(S.numbers_generator);
  exit(EXIT_SUCCESS);
}


/** ------------------------------------------------------------
 ** Iteration functions.
 ** ----------------------------------------------------------*/

static double
alea (annealing_multibest_workspace_t * S)
{
  double	max_step = *((double *)S->max_step_value);

  return (2.0 * gsl_rng_uniform(S->numbers_generator) - 1.0) * max_step;
}

/* ------------------------------------------------------------ */

double
energy_function (void * dummy ANNEALING_UNUSED, void * configuration)
{
  double	C = *((double *)configuration);

  return -sin(C)/C;
}
void
step_function (void * W, void * configuration)
{
  annealing_multibest_workspace_t * S = W;
  double *	C = (double *)configuration;
  double	c;

  do c = *C + alea(S); while (fabs(c) > 120.0);
  *C = c;
}
double
metric_function (void * dummy ANNEALING_UNUSED, void * configuration_a, void * configuration_b)
{
  double	A = *((double *)configuration_a);
  double	B = *((double *)configuration_b);

  return fabs(A - B);
}
void
log_function (void * W)
{
  annealing_multibest_workspace_t * S = W;
  double	current = *((double *)S->current_configuration.data);


  printf("current %5.5g (energy %.4f), worst best energy %.4f, best (%u):",
	 current, S->current_configuration.energy,
	 S->best_configurations[S->best_configurations_count-1].energy,
	 S->best_configurations_count);
  for (size_t i=0; i<S->best_configurations_count; ++i)
    {
      double *	value = S->best_configurations[i].data;
      printf(" %.5f", *value);
    }
  printf("\n");
}


/** ------------------------------------------------------------
 ** Configuration handling functions.
 ** ----------------------------------------------------------*/

void
copy_function (void * dummy ANNEALING_UNUSED, void * dst_configuration, void * src_configuration)
{
  double *	dst = dst_configuration;
  double *	src = src_configuration;

  *dst = *src;
}


/* end of file */
