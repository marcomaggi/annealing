/* gsl_annealing.h --
 * 
 * Copyright (C) 1996, 1997, 1998, 1999, 2000 Mark Galassi
 * Copyright (C) 2007 Marco Maggi
 * 
 * This program is free software;  you can redistribute it and/or modify
 * it under the terms of the  GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is  distributed in the hope that it  will be useful, but
 * WITHOUT  ANY   WARRANTY;  without   even  the  implied   warranty  of
 * MERCHANTABILITY  or FITNESS FOR  A PARTICULAR  PURPOSE.  See  the GNU
 * General Public License for more details.
 * 
 * You should  have received  a copy of  the GNU General  Public License
 * along  with  this  program;  if  not,  write  to  the  Free  Software
 * Foundation,  Inc.,  51  Franklin  Street,  Fifth  Floor,  Boston,  MA
 * 02110-1301, USA.
 */

#ifndef __GSL_ANNEALING_H__
#define __GSL_ANNEALING_H__


/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#include <stdlib.h>
#include <gsl/gsl_rng.h>

#undef __BEGIN_DECLS
#undef __END_DECLS
#ifdef __cplusplus
# define __BEGIN_DECLS extern "C" {
# define __END_DECLS }
#else
# define __BEGIN_DECLS /* empty */
# define __END_DECLS /* empty */
#endif

__BEGIN_DECLS



/** ------------------------------------------------------------
 ** Simple algorithm type definitions.
 ** ----------------------------------------------------------*/

typedef double	gsl_annealing_energy_fun_t	(void * S, void * configuration);
typedef void	gsl_annealing_step_fun_t	(void * S, void * configuration);
typedef double	gsl_annealing_metric_fun_t	(void * S,
						 void * configuration_a,
						 void * configuration_b);
typedef void	gsl_annealing_log_fun_t		(void * S);
typedef void	gsl_annealing_copy_fun_t	(void * S,
						 void * dst_configuration,
						 void * src_configuration);

/* ------------------------------------------------------------ */

typedef struct gsl_annealing_simple_workspace_t {
  size_t	number_of_iterations_at_fixed_temperature;
  void *	max_step_value;

  double	boltzmann_constant;
  double	temperature;		/* initial temperature */
  double	damping_factor;
  double	minimum_temperature;
  double	restart_temperature;

  void *	configuration;
  void *	new_configuration;
  void *	best_configuration;

  double	energy;
  double	new_energy;
  double	best_energy;

  gsl_rng *	numbers_generator;

  gsl_annealing_energy_fun_t *	energy_function;
  gsl_annealing_step_fun_t *	step_function;

  gsl_annealing_log_fun_t *	log_function;

  gsl_annealing_copy_fun_t *	copy_function;

  void *	params;
} gsl_annealing_simple_workspace_t;

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Multi-best algorithm type definitions.
 ** ----------------------------------------------------------*/

typedef struct gsl_annealing_multi_workspace_t {
  size_t		number_of_iterations_at_fixed_temperature;
  void *		max_step_value;
  double		minimum_acceptance_distance;

  double		boltzmann_constant;
  double		temperature;		/* initial temperature */
  double		damping_factor;
  double		minimum_temperature;

  size_t		best_configurations_max;
  size_t		best_configurations_count;

  void *		configuration;
  void *		new_configuration;
  unsigned char **	best_configurations;

  double		energy;
  double		new_energy;
  double *		best_energies;

  gsl_rng *		numbers_generator;

  gsl_annealing_energy_fun_t *	energy_function;
  gsl_annealing_step_fun_t *	step_function;

  gsl_annealing_log_fun_t *	log_function;

  gsl_annealing_copy_fun_t *	copy_function;
  gsl_annealing_metric_fun_t *	metric_function;

  void *		params;
} gsl_annealing_multi_workspace_t;

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Algorithms.
 ** ----------------------------------------------------------*/

extern void gsl_annealing_simple_solve (gsl_annealing_simple_workspace_t * S);
extern void gsl_annealing_multi_solve  (gsl_annealing_multi_workspace_t * S);


__END_DECLS

#endif /* __GSL_ANNEALING_H__ */


/* end of file */
