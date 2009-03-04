/*
   Part of: annealing
   Contents: class definition for simple annealing wrapper
   Date: Thu Jun  5, 2008
   
   Abstract
   
	Defines a virtual class that wraps the basic mechanism
	of simple simulated annealing from Annealing.
   
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

#ifndef SIMPLE_WRAPPER_HPP
#define SIMPLE_WRAPPER_HPP


/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#include <cmath>
#include "numbers_generator.hpp"

extern "C" {

#include <float.h>		/* for 'DBL_MIN' */
#include <unistd.h>		/* for 'getopt()' */
#include <gsl/gsl_rng.h>
#include "annealing.h"

}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Basic simple annealing wrapper.
 ** ----------------------------------------------------------*/

class Annealing_Simple_Config {
public:
  size_t	number_of_iterations_at_fixed_temperature;
  void *	max_step_value;

  double	boltzmann_constant;
  double	temperature;
  double	damping_factor;
  double	minimum_temperature;
  double	restart_temperature;

  Annealing_Simple_Config(void);
};

class Annealing_Simple {
private:
  bool				logging;

  static annealing_energy_fun_t	energy_function_stub;
  static annealing_step_fun_t	step_function_stub;
  static annealing_log_fun_t	log_function_stub;
  static annealing_copy_fun_t	copy_function_stub;

protected:
  annealing_simple_workspace_t	S;

public:
  Annealing_Simple (Annealing_Simple_Config& C, Numbers_Generator& rnd);

  virtual double energy_function(void * configuration) = 0;
  virtual void	step_function	(void * configuration) = 0;

  virtual void	log_function	(void) = 0;
  virtual void	copy_function	(void * dst_configuration,
				 void * src_configuration) = 0;

  void		set_logging	(bool activate);
  void		solve (void);
};

/* ------------------------------------------------------------ */

#endif

/* end of file */
