/*
   Part of: annealing
   Contents: class methods for univariate function minimisation
   Date: Thu Jun  5, 2008
   
   Abstract
   
	Defines the methods required to minimise a univariate
	function.
   
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
#include "univariate_minimisation.hpp"

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Constructors.
 ** ----------------------------------------------------------*/

void
Univariate_Minimisation::initialisation (double first_guess, double _max_step)
{
  max_step = _max_step;

  S.max_step_value		= &max_step;
  S.current_configuration.data	= &(configurations[0]);
  S.best_configuration.data	= &(configurations[1]);
  S.new_configuration.data	= &(configurations[2]);

  configurations[0] = first_guess;
}
Univariate_Minimisation::Univariate_Minimisation (double first_guess, double _max_step = 10.0)
{
  initialisation(first_guess, _max_step);
}
Univariate_Minimisation::Univariate_Minimisation (double first_guess, double _max_step,
						  gsl_rng * numbers_generator)
  : Annealing_Simple (numbers_generator)
{
  initialisation(first_guess, _max_step);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Univariate minimisation methods.
 ** ----------------------------------------------------------*/

double
Univariate_Minimisation::best_estimation (void)
{
  return configurations[1];
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Implementation of base's virtual functions.
 ** ----------------------------------------------------------*/

void
Univariate_Minimisation::step_function (void * configuration)
{
  double *	C = (double *)configuration;
  double	c;
  double	alea;

  do
    {
      alea = (2.0 * gsl_rng_uniform(S.numbers_generator) - 1.0) * max_step;
      c = *C + alea;
    }
  while (fabs(c) > 120.0);
  *C = c;
}
void
Univariate_Minimisation::log_function (void)
{
  double	current = *((double *)S.current_configuration.data);
  double	best    = *((double *)S.best_configuration.data);

  std::cout << "current " << current
	    << " (energy " << S.current_configuration.energy
	    << "), best " << best
	    << " (energy " << S.best_configuration.energy
	    << ")" << std::endl;
}
void
Univariate_Minimisation::copy_function (void * dst_configuration,
					void * src_configuration)
{
  double *	dst = (double *)dst_configuration;
  double *	src = (double *)src_configuration;

  *dst = *src;
}

/* ------------------------------------------------------------ */

/* end of file */
