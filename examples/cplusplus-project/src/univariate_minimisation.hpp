/*
   Part of: annealing
   Contents: class definition for univariate function minimisation
   Date: Thu Feb 22, 2007
   
   Abstract
   
	Declares a class required to minimise a univariate
	function.  It is a specialisation of the basic
	simulated annealing wrapper.
   
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


#ifndef UNIVARIATE_MINIMISATION_HPP
#define UNIVARIATE_MINIMISATION_HPP


/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#include "simple_wrapper.hpp"

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Class definition.
 ** ----------------------------------------------------------*/

class Univariate_Minimisation : public Annealing_Simple
{
private:
  double	configurations[3];
  double	max_step;

protected:
  void	step_function		(void * configuration);

  void	log_function		(void);
  void	copy_function		(void * dst_configuration,
				 void * src_configuration);

public:
  Univariate_Minimisation (double first_guess, double max_step);

  double	best_estimation (void);
};

/* ------------------------------------------------------------ */


#endif

/* end of file */
