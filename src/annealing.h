/* annealing.h --
 *
 * Copyright (C) 2007, 2014, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>
 * Copyright (C) 1996, 1997, 1998, 1999, 2000 Mark Galassi
 *
 * This program is free software: you can  redistribute it and/or modify it under the
 * terms  of  the GNU  General  Public  License as  published  by  the Free  Software
 * Foundation,  either version  3  of the  License,  or (at  your  option) any  later
 * version.
 *
 * This program is  distributed in the hope  that it will be useful,  but WITHOUT ANY
 * WARRANTY; without  even the implied warranty  of MERCHANTABILITY or FITNESS  FOR A
 * PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of  the GNU General Public License along with this
 * program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ANNEALING_H
#define ANNEALING_H


/** --------------------------------------------------------------------
 ** Preliminary definitions.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* The  macro  ANNEALING_UNUSED  indicates  that a  function,  function  argument  or
   variable may potentially be unused. Usage examples:

   static int unused_function (char arg) ANNEALING_UNUSED;
   int foo (char unused_argument ANNEALING_UNUSED);
   int unused_variable ANNEALING_UNUSED;
*/
#ifdef __GNUC__
#  define ANNEALING_UNUSED		__attribute__((__unused__))
#else
#  define ANNEALING_UNUSED		/* empty */
#endif

#ifndef __GNUC__
#  define __attribute__(...)	/* empty */
#endif

/* I found  the following chunk on  the Net.  (Marco Maggi;  Sun Feb 26,
   2012) */
#if defined _WIN32 || defined __CYGWIN__
#  ifdef BUILDING_DLL
#    ifdef __GNUC__
#      define annealing_decl		__attribute__((__dllexport__)) extern
#    else
#      define annealing_decl		__declspec(dllexport) extern
#    endif
#  else
#    ifdef __GNUC__
#      define annealing_decl		__attribute__((__dllimport__)) extern
#    else
#      define annealing_decl		__declspec(dllimport) extern
#    endif
#  endif
#  define annealing_private_decl	extern
#else
#  if __GNUC__ >= 4
#    define annealing_decl		__attribute__((__visibility__("default"))) extern
#    define annealing_private_decl	__attribute__((__visibility__("hidden")))  extern
#  else
#    define annealing_decl		extern
#    define annealing_private_decl	extern
#  endif
#endif


/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#include <stdlib.h>
#include <gsl/gsl_rng.h>


/** ------------------------------------------------------------
 ** Version functions.
 ** ----------------------------------------------------------*/

annealing_decl int annealing_version_interface_current (void);
annealing_decl int annealing_version_interface_revision (void);
annealing_decl int annealing_version_interface_age (void);
annealing_decl const char * annealing_version_string (void);


/** ------------------------------------------------------------
 ** Generic data types.
 ** ----------------------------------------------------------*/

typedef struct annealing_configuration_t {
  void *	data;
  double	energy;
} annealing_configuration_t;

typedef double	annealing_energy_fun_t	(void * S, void * configuration);
typedef void	annealing_step_fun_t	(void * S, void * configuration);
typedef double	annealing_metric_fun_t	(void * S,
					 void * configuration_a,
					 void * configuration_b);
typedef void	annealing_cooling_fun_t	(void * S);
typedef void	annealing_log_fun_t	(void * S);
typedef void	annealing_copy_fun_t	(void * S,
					 void * dst_configuration,
					 void * src_configuration);


/** ------------------------------------------------------------
 ** Simple algorithm type definitions.
 ** ----------------------------------------------------------*/

typedef struct annealing_simple_workspace_t {
  size_t	number_of_iterations_at_fixed_temperature;
  void *	max_step_value;

  double	boltzmann_constant;
  double	temperature;		/* initial temperature */
  double	damping_factor;
  double	minimum_temperature;
  double	restart_temperature;
  int		restart_flag;

  annealing_configuration_t	current_configuration;
  annealing_configuration_t	new_configuration;
  annealing_configuration_t	best_configuration;

  gsl_rng *	numbers_generator;

  annealing_energy_fun_t *	energy_function;
  annealing_step_fun_t *	step_function;
  annealing_log_fun_t *		log_function;
  annealing_copy_fun_t *	copy_function;
  annealing_cooling_fun_t *	cooling_function;

  void *	params;
} annealing_simple_workspace_t;



/** ------------------------------------------------------------
 ** Multi-best algorithm type definitions.
 ** ----------------------------------------------------------*/

typedef struct annealing_multibest_workspace_t {
  size_t	number_of_iterations_at_fixed_temperature;
  void *	max_step_value;
  double	minimum_acceptance_distance;

  double	boltzmann_constant;
  double	temperature;		/* initial temperature */
  double	damping_factor;
  double	minimum_temperature;

  annealing_configuration_t	current_configuration;
  annealing_configuration_t	new_configuration;
  annealing_configuration_t *	best_configurations;
  size_t	max_number_of_best_configurations;
  size_t	best_configurations_count;

  gsl_rng *	numbers_generator;

  annealing_energy_fun_t *	energy_function;
  annealing_step_fun_t *	step_function;
  annealing_log_fun_t *		log_function;
  annealing_copy_fun_t *	copy_function;
  annealing_metric_fun_t *	metric_function;
  annealing_cooling_fun_t *	cooling_function;

  void *		params;
} annealing_multibest_workspace_t;


/** ------------------------------------------------------------
 ** Many tries algorithm type definitions.
 ** ----------------------------------------------------------*/

typedef struct annealing_manytries_workspace_t {
  void *	max_step_value;

  double	boltzmann_constant;
  double	temperature;		/* initial temperature */
  double	damping_factor;
  double	minimum_temperature;
  double	restart_temperature;
  int		restart_flag;

  annealing_configuration_t	current_configuration;
  annealing_configuration_t	best_configuration;
  annealing_configuration_t *	new_configurations;
  double *	monte_carlo_coordinates;
  size_t	number_of_tries;

  gsl_rng *	numbers_generator;

  annealing_energy_fun_t *	energy_function;
  annealing_step_fun_t *	step_function;
  annealing_log_fun_t *		log_function;
  annealing_copy_fun_t *	copy_function;
  annealing_cooling_fun_t *	cooling_function;

  void *		params;
} annealing_manytries_workspace_t;


/** ------------------------------------------------------------
 ** Algorithms.
 ** ----------------------------------------------------------*/

annealing_decl void annealing_simple_solve    (annealing_simple_workspace_t * S);
annealing_decl void annealing_multibest_solve (annealing_multibest_workspace_t * S);
annealing_decl void annealing_manytries_solve (annealing_manytries_workspace_t * S);


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* ANNEALING_H */

/* end of file */
