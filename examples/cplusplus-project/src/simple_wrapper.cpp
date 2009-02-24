/*
   Part of: annealing
   Contents: class definition for simple annealing wrapper
   Date: Thu Jun  5, 2008
   
   Abstract
   
	Declares   the  methods   of  the   basic  simulated
	annealing algorithm from Annealing.
   
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
 ** Setup.
 ** ----------------------------------------------------------*/

#include "internal.hpp"
#include "simple_wrapper.hpp"

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Constructors.
 ** ----------------------------------------------------------*/

Annealing_Simple_Config::Annealing_Simple_Config(void)
{
  number_of_iterations_at_fixed_temperature = 10;
  temperature			= 10.0;
  minimum_temperature		= 1.0e-6;
  restart_temperature		= DBL_MIN; /* do not restart */
  boltzmann_constant		= 1.0;
  damping_factor		= 1.005;
}

Annealing_Simple::Annealing_Simple (Annealing_Simple_Config& C, Numbers_Generator& rnd)
{
  S.energy_function		= energy_function_stub;
  S.step_function		= step_function_stub;

  S.copy_function		= copy_function_stub;
  S.log_function		= log_function_stub;
  S.cooling_function		= NULL;

  S.numbers_generator		= rnd.get_pointer();

  S.number_of_iterations_at_fixed_temperature = C.number_of_iterations_at_fixed_temperature;
  S.temperature			= C.temperature;
  S.minimum_temperature		= C.minimum_temperature;
  S.restart_temperature		= C.restart_temperature; /* do not restart */
  S.boltzmann_constant		= C.boltzmann_constant;
  S.damping_factor		= C.damping_factor;

  S.params = this;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Methods.
 ** ----------------------------------------------------------*/

void
Annealing_Simple::solve (void)
{
  annealing_simple_solve(&S);
}
void
Annealing_Simple::set_logging (bool active)
{
  logging = active;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Stub callback functions.
 ** ----------------------------------------------------------*/

double
Annealing_Simple::energy_function_stub (void * _S, void * configuration)
{
  annealing_simple_workspace_t *	S = (annealing_simple_workspace_t *)_S;
  Annealing_Simple *			A = (Annealing_Simple *)(S->params);

  return A->energy_function(configuration);
}
void
Annealing_Simple::step_function_stub (void * _S, void * configuration)
{
  annealing_simple_workspace_t *	S = (annealing_simple_workspace_t *)_S;
  Annealing_Simple *			A = (Annealing_Simple *)(S->params);

  A->step_function(configuration);
}

/* ------------------------------------------------------------ */

void
Annealing_Simple::log_function_stub (void * _S)
{
  annealing_simple_workspace_t *	S = (annealing_simple_workspace_t *)_S;
  Annealing_Simple *			A = (Annealing_Simple *)(S->params);

  if (A->logging)
    {
      return A->log_function();
    }
}
void
Annealing_Simple::copy_function_stub (void * _S,
				      void * dst_configuration, void * src_configuration)
{
  annealing_simple_workspace_t *	S = (annealing_simple_workspace_t *)_S;
  Annealing_Simple *			A = (Annealing_Simple *)(S->params);

  return A->copy_function(dst_configuration, src_configuration);
}

/* ------------------------------------------------------------ */

/* end of file */
