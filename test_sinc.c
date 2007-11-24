/*
   Part of: annealing
   Contents: annealing test with sinc(t) minimisation
   Date: Thu Feb 22, 2007
   
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

int
main (int argc, char ** argv)
{
  gsl_annealing_simple_workspace_t	S;
  double	configurations[3];
  double	max_step = 5.0;
  int		verbose_mode = 0;
  

  {
    int c;

    while ((c = getopt(argc, argv, "hv")) != -1)
      switch (c)
	{
	case 'h':
	  fprintf(stderr, "usage: test_sinc [-v] [-h]\n");
	  goto exit;
	case 'v':
	  verbose_mode = 1;
	  break;
	default:
	  fprintf(stderr, "test_sinc error: unknown option %c\n", c);
	  exit(EXIT_FAILURE);
	}
  }

  printf("\n------------------------------------------------------------\n");
  printf("test_sinc: sinc minimisation with simulated annealing\n");

  S.number_of_iterations_at_fixed_temperature = 10;
  S.max_step_value	= &max_step;

  S.temperature		= 0.02;
  S.minimum_temperature	= 1.0e-6;
  S.restart_temperature	= DBL_MIN; /* do not restart */
  S.boltzmann_constant	= 1.0;
  S.damping_factor	= 1.005;

  S.energy_function	= energy_function;
  S.step_function	= step_function;
  S.copy_function	= copy_function;
  S.log_function	= (verbose_mode)? log_function : NULL;

  S.numbers_generator	= gsl_rng_alloc(gsl_rng_rand);
  gsl_rng_set(S.numbers_generator, 15);

  S.configuration	= &(configurations[0]);
  S.best_configuration	= &(configurations[1]);
  S.new_configuration	= &(configurations[2]);

  configurations[0] = 100.0;

  gsl_annealing_simple_solve(&S);

  printf("test_sinc: final best solution: %f, global 0.0\n", configurations[1]);
  printf("------------------------------------------------------------\n\n");

  gsl_rng_free(S.numbers_generator);
 exit:
  exit(EXIT_SUCCESS);
}


/** ------------------------------------------------------------
 ** Iteration functions.
 ** ----------------------------------------------------------*/

double
energy_function (void * dummy, void * configuration)
{
  double	C = *((double *)configuration);

  return -sin(C)/C;
}
void
step_function (void * W, void * configuration)
{
  gsl_annealing_simple_workspace_t * S = W;
  double *	C = (double *)configuration;

  *C += (2.0 * gsl_rng_uniform(S->numbers_generator) - 1.0) *
    *((double *)S->max_step_value);
}
void
log_function (void * W)
{
  gsl_annealing_simple_workspace_t * S = W;

  printf("current %f (energy %f), best %f (energy %f)\n",
	 *((double *)S->configuration), energy_function(S, S->configuration),
	 *((double *)S->best_configuration), energy_function(S, S->best_configuration));
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
