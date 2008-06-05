/*
   Part of: annealing
   Contents: class definition for simple annealing wrapper
   Date: Thu Jun  5, 2008
   
   Abstract
   

   
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

#include "internal.h"

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Module declarations.
 ** ----------------------------------------------------------*/

static	annealing_energy_fun_t	energy_function_stub;
static	annealing_step_fun_t	step_function_stub;
static	annealing_log_fun_t	log_function_stub;
static	annealing_copy_fun_t	copy_function_stub;

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Constructors.
 ** ----------------------------------------------------------*/

Annealing_Simple::Annealing_Simple (gsl_rng * numbers_generator)
{
  S = new(struct annealing_simple_workspace_t);
  initialisation(numbers_generator);
}
Annealing_Simple::Annealing_Simple (void)
{
  gsl_rng *		numbers_generator;

  S = new(annealing_simple_workspace_t);
  numbers_generator = gsl_rng_alloc(gsl_rng_rand);
  gsl_rng_set(S->numbers_generator, 15);

  initialisation(numbers_generator);
}
Annealing_Simple::~Annealing_Simple (void)
{
  gsl_rng_free(S->numbers_generator);
  delete S;
}

/* ------------------------------------------------------------ */

void
Annealing_Simple::initialisation (gsl_rng * numbers_generator)
{
  S->energy_function	= energy_function_stub;
  S->step_function	= step_function_stub;

  S->copy_function	= copy_function_stub;
  S->log_function	= log_function_stub;
  S->cooling_function	= NULL;

  S->numbers_generator	= numbers_generator;

  S->number_of_iterations_at_fixed_temperature = 10;
  S->temperature		= 10.0;
  S->minimum_temperature	= 1.0e-6;
  S->restart_temperature	= DBL_MIN; /* do not restart */
  S->boltzmann_constant		= 1.0;
  S->damping_factor		= 1.005;

  S->params = this;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Methods.
 ** ----------------------------------------------------------*/

void
Annealing_Simple::solve (void)
{
  annealing_simple_solve(S);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Stub callback functions.
 ** ----------------------------------------------------------*/

double
energy_function_stub (void * _S, void * configuration)
{
  annealing_simple_workspace_t *	S = (annealing_simple_workspace_t *)_S;
  Annealing_Simple *			A = (Annealing_Simple *)(S->params);

  return A->energy_function(configuration);
}
void
step_function_stub (void * _S, void * configuration)
{
  annealing_simple_workspace_t *	S = (annealing_simple_workspace_t *)_S;
  Annealing_Simple *			A = (Annealing_Simple *)(S->params);

  A->step_function(configuration);
}

/* ------------------------------------------------------------ */

void
log_function_stub (void * _S)
{
  annealing_simple_workspace_t *	S = (annealing_simple_workspace_t *)_S;
  Annealing_Simple *			A = (Annealing_Simple *)(S->params);

  return A->log_function();
}
void
copy_function_stub (void * _S,
		    void * dst_configuration, void * src_configuration)
{
  annealing_simple_workspace_t *	S = (annealing_simple_workspace_t *)_S;
  Annealing_Simple *			A = (Annealing_Simple *)(S->params);

  return A->copy_function(dst_configuration, src_configuration);
}

/* ------------------------------------------------------------ */

/* end of file */
