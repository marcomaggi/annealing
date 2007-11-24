/*
   Part of: annealing
   Contents: annealing test with dumped bidimensional sin minimisation
   Date: Thu Feb 22, 2007
   
   Abstract
   
	Find the minimum of 'f(t) = -sin(sqrt(X*X + Y*Y))/(X*X + Y*Y)'.
   
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
#include "annealing.h"


static	annealing_energy_fun_t	energy_function;
static	annealing_step_fun_t	step_function;
static	annealing_log_fun_t		log_function;
static	annealing_copy_fun_t	copy_function;


/* Why do I have to do this even if I have included "unistd.h"? */
extern int getopt (int argc, char ** argv, const char * options);



/** ------------------------------------------------------------
 ** Main.
 ** ----------------------------------------------------------*/

int
main (int argc, char ** argv)
{
  annealing_simple_workspace_t	S;
  double	configurations[3][2];
  double	max_step = 100.0;
  int		verbose_mode = 0;
  

  {
    int c;

    while ((c = getopt(argc, argv, "hv")) != -1)
      switch (c)
	{
	case 'h':
	  fprintf(stderr, "usage: test_sinxy [-v] [-h]\n");
	  goto exit;
	case 'v':
	  verbose_mode = 1;
	  break;
	default:
	  fprintf(stderr, "test_sinxy error: unknown option %c\n", c);
	  exit(EXIT_FAILURE);
	}
  }

  printf("\n------------------------------------------------------------\n");
  printf("test_sinxy: sinxy minimisation with simulated annealing\n");

  S.number_of_iterations_at_fixed_temperature = 100;
  S.max_step_value	= &max_step;

  S.temperature		= 60.0;
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

  S.current_configuration.data	= &(configurations[0]);
  S.best_configuration.data	= &(configurations[1]);
  S.new_configuration.data	= &(configurations[2]);

  configurations[0][0] = 15.0;
  configurations[0][1] = 15.0;

  annealing_simple_solve(&S);

  printf("test_sinxy: final best solution: %f,%f; global 0.0,0.0\n",
	 configurations[1][0], configurations[1][1]);
  printf("------------------------------------------------------------\n\n");

  gsl_rng_free(S.numbers_generator);
 exit:
  exit(EXIT_SUCCESS);
}


/** ------------------------------------------------------------
 ** Iteration functions.
 ** ----------------------------------------------------------*/

static double
alea (annealing_simple_workspace_t * S)
{
  return (2.0 * gsl_rng_uniform(S->numbers_generator) - 1.0) *
    *((double *)S->max_step_value);
}

/* ------------------------------------------------------------ */

double
energy_function (void * dummy, void * configuration)
{
  double *	C = configuration;
  double	X = C[0], Y = C[1];

  return -sin(sqrt(X*X + Y*Y))/(X*X + Y*Y);
}
void
step_function (void * W, void * configuration)
{
  annealing_simple_workspace_t * S = W;
  double *	C = configuration;
  double	c0, c1;

  do c0 = C[0] + alea(S); while (fabs(c0) > 30.0);
  C[0] = c0;
  do c1 = C[1] + alea(S); while (fabs(c1) > 30.0);
  C[1] = c1;
}
void
log_function (void * W)
{
  annealing_simple_workspace_t * S = W;
  double *	C = S->current_configuration.data;
  double *	B = S->best_configuration.data;

  printf("current %f,%f (energy %f), best %f,%f (energy %f)\n",
	 C[0], C[1], S->current_configuration.energy,
	 B[0], B[1], S->best_configuration.energy);
}


/** ------------------------------------------------------------
 ** Configuration handling functions.
 ** ----------------------------------------------------------*/

void
copy_function (void * dummy, void * dst_configuration, void * src_configuration)
{
  double *	dst = dst_configuration;
  double *	src = src_configuration;

  dst[0] = src[0];
  dst[1] = src[1];
}


/* end of file */
