/*
   Part of: annealing
   Contents: annealing test with parameters fitting
   Date: Fri Feb 23, 2007

   Abstract

	Find the best fit against a vector of sample for the exponential
	model: 'y(t)  = A *  exp(-lam * t)  + b', with  parameters: 'A',
	'lam', 'b'.

   Copyright (c) 2007, 2019 Marco Maggi

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

/* Enable latest POSIX features. */
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE		200809L

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <gsl/gsl_rng.h>
#include "annealing.h"


static	annealing_energy_fun_t	energy_function;
static	annealing_step_fun_t	step_function;
static	annealing_log_fun_t	log_function;
static	annealing_copy_fun_t	copy_function;


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

typedef struct fitting_step_t {
  double	A;
  double	lambda;
  double	b;
} fitting_step_t;

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
  annealing_simple_workspace_t	S;
  fitting_data_t	D;
  fitting_step_t	max_step = { 1.0, 0.1, 1.0 };
  configuration_t	configurations[3];
  int			verbose_mode = 0;


  {
    int c;

    while ((c = getopt(argc, argv, "hv")) != -1)
      switch (c)
	{
	case 'h':
	  fprintf(stderr, "usage: test_fitting [-v] [-h]\n");
	  goto exit;
	case 'v':
	  verbose_mode = 1;
	  break;
	default:
	  fprintf(stderr, "test_fitting error: unknown option %c\n", c);
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
    S.cooling_function		= NULL;

    S.numbers_generator		= gsl_rng_alloc(gsl_rng_rand);
    gsl_rng_set(S.numbers_generator, 15);

    S.current_configuration.data= &(configurations[0]);
    S.best_configuration.data	= &(configurations[1]);
    S.new_configuration.data	= &(configurations[2]);

    /* start configuration */
    configurations[0].A		= 6.0;
    configurations[0].lambda	= 3.0;
    configurations[0].b		= 1.0;

    S.params			= &D;
  }

  annealing_simple_solve(&S);

  printf("test_fitting: final best solution: %f, %f, %f; original: %g, %g, %g\n",
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
alea (annealing_simple_workspace_t * S, double max_step)
{
  return (2.0 * gsl_rng_uniform(S->numbers_generator) - 1.0) * max_step;
}

/* ------------------------------------------------------------ */

double
energy_function (void * W, void * configuration)
{
  annealing_simple_workspace_t * S = W;
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
step_function (void * W, void * configuration)
{
  annealing_simple_workspace_t * S = W;
  configuration_t *	C = configuration;
  fitting_step_t *	step = S->max_step_value;

  C->A		+= alea(S, step->A);
  C->lambda	+= alea(S, step->lambda);
  C->b		+= alea(S, step->b);
}
void
log_function (void * W)
{
  annealing_simple_workspace_t * S = W;
  configuration_t *	C = S->current_configuration.data;
  configuration_t *	B = S->best_configuration.data;

  printf("current: %f, %f, %f (energy %f); best: %f, %f, %f (energy %f)\n",
	 C->A, C->lambda, C->b, S->current_configuration.energy,
	 B->A, B->lambda, B->b, S->best_configuration.energy);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Configuration handling functions.
 ** ----------------------------------------------------------*/

void
copy_function (void * dummy ANNEALING_UNUSED, void * dst_configuration, void * src_configuration)
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
