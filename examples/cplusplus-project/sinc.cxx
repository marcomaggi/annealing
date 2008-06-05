/*
   Part of: annealing
   Contents: annealing test with sinc(t) minimisation
   Date: Thu Feb 22, 2007
   
   Abstract
   
	Find the minimum of 'f(t) = -sin(t)/t'.
   
   Copyright (c) 2007, 2008 Marco Maggi
   
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

#include <iostream>
#include <cmath>

extern "C" {

#include <float.h>		/* for 'DBL_MIN' */
#include <unistd.h>		/* for 'getopt()' */
#include <gsl/gsl_rng.h>
#include "annealing.h"

}

static	annealing_energy_fun_t	energy_function;
static	annealing_step_fun_t	step_function;
static	annealing_log_fun_t	log_function;
static	annealing_copy_fun_t	copy_function;

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Main.
 ** ----------------------------------------------------------*/

int
main (int argc, char ** argv)
{
  annealing_simple_workspace_t	S;
  double	configurations[3];
  double	max_step = 10.0;
  int		verbose_mode = 0;
  

  {
    int c;

    while ((c = getopt(argc, argv, "hv")) != -1)
      switch (c)
	{
	case 'h':
	  fprintf(stderr, "usage: test_sinc [-v] [-h]\n");
	  exit(EXIT_SUCCESS);
	case 'v':
	  verbose_mode = 1;
	  break;
	default:
	  fprintf(stderr, "test_sinc error: unknown option %c\n", c);
	  exit(EXIT_FAILURE);
	}
  }

  std::cout << std::endl
	    << "\n------------------------------------------------------------" << std::endl;
  printf("test_sinc: sinc minimisation with simulated annealing\n");

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
  S.cooling_function		= NULL;

  S.numbers_generator		= gsl_rng_alloc(gsl_rng_rand);
  gsl_rng_set(S.numbers_generator, 15);

  S.current_configuration.data	= &(configurations[0]);
  S.best_configuration.data	= &(configurations[1]);
  S.new_configuration.data	= &(configurations[2]);

  configurations[0] = 100.0;

  annealing_simple_solve(&S);

  std::cout << "test_sinc: final best solution: " configurations[1] ", global 0.0"
	    << std::endl
	    << "------------------------------------------------------------"
	    << std::endl << std::endl;

  gsl_rng_free(S.numbers_generator);
  exit(EXIT_SUCCESS);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Iteration functions.
 ** ----------------------------------------------------------*/

static double
alea (annealing_simple_workspace_t * S)
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
  annealing_simple_workspace_t * S = W;
  double *	C = (double *)configuration;
  double	c;

  do c = *C + alea(S); while (fabs(c) > 120.0);
  *C = c;
}
void
log_function (void * W)
{
  annealing_simple_workspace_t * S = W;
  double	current = *((double *)S->current_configuration.data);
  double	best    = *((double *)S->best_configuration.data);

  printf("current %f (energy %f), best %f (energy %f)\n",
	 current, S->current_configuration.energy,
	 best,    S->best_configuration.energy);
}

/* ------------------------------------------------------------ */


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
