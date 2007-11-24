/*
   Part of: annealing
   Contents: annealing test with parameters fitting
   Date: Fri Feb 23, 2007
   
   Abstract
   
   
   
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
#include <gsl/gsl_rng.h>
#include "gsl_annealing.h"


static	gsl_annealing_energy_fun_t	energy_function;
static	gsl_annealing_step_fun_t	step_function;
static	gsl_annealing_log_fun_t		log_function;
static	gsl_annealing_copy_fun_t	copy_function;


/* Why do I have to do this even if I have included "unistd.h"? */
extern int getopt (int argc, char ** argv, const char * options);

/* ------------------------------------------------------------ */

#define NUM		600

typedef struct configuration_t {
  double	A;
  double	lambda;
  double	b;
} configuration_t;

typedef struct fitting_data_t {
  size_t	num;
  double	t[NUM];
  double	observations[NUM];
} fitting_data_t;

static const configuration_t original_params = { 5.0, 0.1, 7.0 };

static void make_observations	(double * t, double * observations, size_t num);
static void linspace		(double * t, size_t num, double min, double max);
static double y			(double t, const configuration_t * params);

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Main.
 ** ----------------------------------------------------------*/

int
main (int argc, char ** argv)
{
  gsl_annealing_simple_workspace_t	S;
  fitting_data_t	D;
  configuration_t	configurations[3];
  double		max_step = 1.0;
  int			verbose_mode = 0;
  

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
  printf("test_fitting: exponential parameters fitting with simulated annealing\n");
  
  /* fitting data initialisation */
  {
    D.num = NUM;
    linspace(D.t, D.num, 0.0, 10.0);
    make_observations(D.t, D.observations, D.num);
  }

  /* annealing workspace initialisation */
  {
    S.number_of_iterations_at_fixed_temperature = 10;
    S.max_step_value		= &max_step;

    S.temperature		= 10.0;
    S.minimum_temperature	= 0.1;
    S.restart_temperature	= 1.0;
    S.boltzmann_constant	= 1.0;
    S.damping_factor		= 1.005;

    S.energy_function		= energy_function;
    S.step_function		= step_function;
    S.copy_function		= copy_function;
    S.log_function		= (verbose_mode)? log_function : NULL;

    S.numbers_generator		= gsl_rng_alloc(gsl_rng_rand);
    gsl_rng_set(S.numbers_generator, 15);

    S.configuration		= &(configurations[0]);
    S.best_configuration	= &(configurations[1]);
    S.new_configuration		= &(configurations[2]);

    /* start configuration */
    configurations[0].A		= 6.0;
    configurations[0].lambda	= 3.0;
    configurations[0].b		= 1.0;

    S.params			= &D;
  }

  gsl_annealing_simple_solve(&S);

  printf("test_sinxy: final best solution: %f, %f, %f; originall: %g, %g, %g\n",
	 configurations[1].A, configurations[1].lambda, configurations[1].b,
	 original_params.A, original_params.lambda, original_params.b);
  printf("------------------------------------------------------------\n\n");

  gsl_rng_free(S.numbers_generator);
 exit:
  exit(EXIT_SUCCESS);
}


/** ------------------------------------------------------------
 ** Iteration functions.
 ** ----------------------------------------------------------*/

static double
alea (gsl_annealing_simple_workspace_t * S)
{
  return (2.0 * gsl_rng_uniform(S->numbers_generator) - 1.0) *
    *((double *)S->max_step_value);
}

/* ------------------------------------------------------------ */

double
energy_function (gsl_annealing_simple_workspace_t * S, void * configuration)
{
  configuration_t *	C = configuration;
  fitting_data_t *	D = S->params;
  double		norm = 0.0, delta;

  for (size_t i=0; i<D->num; ++i)
    {
      delta = y(D->t[i], C) - D->observations[i];
      norm += delta * delta;
    }
  return norm;
}
void
step_function (gsl_annealing_simple_workspace_t * S, void * configuration)
{
  configuration_t *	C = configuration;

  C->A		+= alea(S);
  C->lambda	+= alea(S) * 0.1;
  C->b		+= alea(S);
}
void
log_function (gsl_annealing_simple_workspace_t * S)
{
  configuration_t *	C = S->configuration;
  configuration_t *	B = S->best_configuration;

  printf("current: %f, %f, %f (energy %f); best: %f, %f, %f (energy %f)\n",
	 C->A, C->lambda, C->b, energy_function(S, S->configuration),
	 B->A, B->lambda, B->b, energy_function(S, S->best_configuration));
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Configuration handling functions.
 ** ----------------------------------------------------------*/

void
copy_function (gsl_annealing_simple_workspace_t	* dummy,
	       void * dst_configuration, void * src_configuration)
{
  configuration_t *	dst = dst_configuration;
  configuration_t *	src = src_configuration;

  *dst = *src;
}


/** ------------------------------------------------------------
 ** Problem functions.
 ** ----------------------------------------------------------*/

double
y (double t, const configuration_t * params)
{
  return (params->A * exp(-params->lambda * t) + params->b);
}

/* ------------------------------------------------------------ */

void
linspace (double * t, size_t num, double beg, double end)
{
  double step = (end - beg) / (num-1);
  
  *t = beg;
  for (size_t i=1; i<num; ++i, ++t)
    t[1] = *t + step;
}
void
make_observations (double * t, double * observations, size_t num)
{
  gsl_rng *	noise_generator;


  noise_generator = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(noise_generator, 10);

  for (size_t i=0; i<num; ++i)
    {
      observations[i] = y(t[i], &original_params) +
	2.0 * gsl_rng_uniform(noise_generator) - 1.0;
    }
  gsl_rng_free(noise_generator);
}




/* end of file */
