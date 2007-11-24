/*
   Part of: annealing
   Contents: annealing test with traveling salesman problem
   Date: Fri Feb 23, 2007
   
   Abstract
   
	Solve the TSP for 12 cities.
   
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
#include <string.h>
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

/* ------------------------------------------------------------ */

#define NUM		12

/* Latitude and  longitude are  obtained from the  US Census  Bureau, at
   <http://www.census.gov/cgi-bin/gazetteer>. */

static const char * city_names[NUM] = {
  "Santa Fe", "Phoenix", "Albuquerque", "Clovis", "Durango", "Dallas",
  "Tesuque", "Grants", "Los Alamos", "Las Cruces", "Cortez", "Gallup" 
};
static const double city_latitudes[NUM] = {
  35.68, 33.54, 35.12,  34.41, 37.29, 32.79,
  35.77, 35.15, 35.89,  32.34, 37.35, 35.52
};
static const double city_longitudes[NUM] = {
  105.95, 112.07, 106.62,  103.20, 107.87, 96.77,
  105.92, 107.84, 106.28,  106.76, 108.58, 108.74
};

/* ------------------------------------------------------------ */

typedef size_t configuration_t[NUM];

typedef struct tsp_data_t {
  size_t	num;
  double * 	distance_matrix;
} tsp_data_t;

static void make_distance_matrix (size_t num,
				  const double * latitudes, const double * longitudes,
				  double * distance_matrix);

static void print_distance_matrix (double * matrix, size_t num);

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Main.
 ** ----------------------------------------------------------*/

int
main (int argc, char ** argv)
{
  gsl_annealing_simple_workspace_t	S;
  tsp_data_t		D;
  double		matrix[NUM * NUM];
  size_t		max_step = 5;
  configuration_t	configurations[3];
  int			verbose_mode = 0;
  

  {
    int c;

    while ((c = getopt(argc, argv, "hv")) != -1)
      switch (c)
	{
	case 'h':
	  fprintf(stderr, "usage: test_tsp [-v] [-h]\n");
	  goto exit;
	case 'v':
	  verbose_mode = 1;
	  break;
	default:
	  fprintf(stderr, "test_tsp error: unknown option %c\n", c);
	  exit(EXIT_FAILURE);
	}
  }

  printf("\n------------------------------------------------------------\n");
  printf("test_tsp: traveling salesman problem with simulated annealing\n");
  
  /* problem data */
  {
    D.num = NUM;
    D.distance_matrix = matrix;
    make_distance_matrix(D.num, city_latitudes, city_longitudes, D.distance_matrix);
  }

  printf("distance matrix:\n");
  printf("---------------------------------\n");
  print_distance_matrix(D.distance_matrix, D.num);
  printf("---------------------------------\n");

  /* annealing workspace initialisation */
  {
    S.number_of_iterations_at_fixed_temperature = 20;
    S.max_step_value		= &max_step;

    S.temperature		= 20.0;
    S.minimum_temperature	= 2.0e-6;
    S.restart_temperature	= DBL_MIN; /* do not restart */
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
    for (size_t i=0; i<D.num; ++i)
      configurations[0][i] = i;	

    S.params			= &D;
  }

  gsl_annealing_simple_solve(&S);

  printf("test_tsp: final best route:");
  for (size_t i=0; i<D.num; ++i) printf(" %d", configurations[1][i]);
  printf("; energy %.1f\n", energy_function(&S, configurations[1]));
  printf("test_tsp:");
  for (size_t i=0; i<D.num; ++i) printf(" %s,", city_names[configurations[1][i]]);
  printf(" and back to %s\n", city_names[configurations[1][0]]);
  printf("------------------------------------------------------------\n\n");

  gsl_rng_free(S.numbers_generator);
 exit:
  exit(EXIT_SUCCESS);
}


/** ------------------------------------------------------------
 ** Iteration functions.
 ** ----------------------------------------------------------*/

double
energy_function (void * W, void * configuration)
{
  gsl_annealing_simple_workspace_t * S = W;
  size_t *	C = configuration;
  tsp_data_t *	D = S->params;
  double	energy = 0.0;
  size_t	i;

  for (i=0; i<D->num; ++i)
    {
      energy += D->distance_matrix[C[i] * D->num + C[(i+1)%D->num]];
    }
  return energy;
}
void
step_function (void * W, void * configuration)
{
  gsl_annealing_simple_workspace_t * S = W;
  size_t *	C = configuration;
  tsp_data_t *	D = S->params;
  double	step = *((size_t *)S->max_step_value);
  long		idx1, idx2, number_of_swaps;
  size_t	tmp;


  do
    number_of_swaps = gsl_rng_uniform_int(S->numbers_generator, step);
  while (0 == number_of_swaps);

  for (long i=0; i<number_of_swaps; ++i)
    {
      idx1 = gsl_rng_uniform_int(S->numbers_generator, D->num);
      idx2 = gsl_rng_uniform_int(S->numbers_generator, D->num);
      if (idx1 == idx2)
	idx2 = (idx1 + 1) % D->num;
      tmp = C[idx1]; C[idx1] = C[idx2]; C[idx2] = tmp;
    }
}
void
log_function (void * W)
{
  gsl_annealing_simple_workspace_t * S = W;
  tsp_data_t *	D = S->params;
  size_t *	C = S->configuration;
  size_t *	B = S->best_configuration;
  size_t *	N = S->new_configuration;

  printf("current: ");
  for (size_t i=0; i<D->num; ++i) printf("%d,", C[i]);
  printf(" (E %.1f)", energy_function(S, C));
  printf("; best: ");
  for (size_t i=0; i<D->num; ++i) printf("%d,", B[i]);
  printf(" (E %.1f)", energy_function(S, B));
  printf("; new: ");
  for (size_t i=0; i<D->num; ++i) printf("%d,", N[i]);
  printf(" (E %.1f)\n", energy_function(S, N));
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Configuration handling functions.
 ** ----------------------------------------------------------*/

void
copy_function (void * W, void * dst_configuration, void * src_configuration)
{
  gsl_annealing_simple_workspace_t * S = W;
  tsp_data_t *	D = S->params;
  size_t *	dst = dst_configuration;
  size_t *	src = src_configuration;

  memcpy(dst, src, sizeof(size_t) * D->num);
}


/** ------------------------------------------------------------
 ** Problem functions.
 ** ----------------------------------------------------------*/

static double
deg2rad (double angle)
{
  return  (2.0 * 3.14159265358979323846 * (angle / 360.0));
}
static double
city_distance (size_t idx1, size_t idx2,
	       const double * latitudes, const double * longitudes)
{
  if (idx1 == idx2)
    return 0.0;
  else
    {
      static const double earth_radius = 6375.0;
      double	lat1, lat2, lon1, lon2;
      double	sla1, sla2, cla1, cla2, slo1, slo2, clo1, clo2;

      lat1 = deg2rad(latitudes[idx1]);
      lat2 = deg2rad(latitudes[idx2]);
      lon1 = deg2rad(longitudes[idx1]);
      lon2 = deg2rad(longitudes[idx2]);

      sla1 = sin(lat1);
      sla2 = sin(lat2);
      slo1 = sin(lon1);
      slo2 = sin(lon2);
      cla1 = cos(lat1);
      cla2 = cos(lat2);
      clo1 = cos(lon1);
      clo2 = cos(lon2);

      return (earth_radius * acos((cla1 * clo1 * cla2 * clo2) +
				  (cla1 * slo1 * cla2 * slo2) +
				  (sla1 * sla2)));
    }
}
void
make_distance_matrix (size_t num,
		      const double * latitudes, const double * longitudes,
		      double * distance_matrix)
{
  for (size_t row=0; row<num; ++row)
    for (size_t col=0; col<num; ++col)
      {
	distance_matrix[row * num + col] =
	  city_distance(row, col, latitudes, longitudes);
      }
}
void
print_distance_matrix (double * matrix, size_t num)
{
  for (size_t row=0; row<num; ++row)
    {
      for (size_t col=0; col<num; ++col)
	{
	  printf("%6.1f ", matrix[row * num + col]);
	}
      printf("\n");
    }
}


/* end of file */
