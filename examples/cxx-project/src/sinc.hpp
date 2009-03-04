/*
   Part of: annealing
   Contents: class definitions for sinc minimisation.
   Date: Thu Feb 22, 2007
   
   Abstract
   
	Defines a class to minimise 'f(t) = -sin(t)/t'.  It is
	a specialisation of the univariate minimisation class.
   
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


#ifndef SINC_HPP
#define SINC_HPP


/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#include "univariate_minimisation.hpp"

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Class definition.
 ** ----------------------------------------------------------*/

class Sinc_Minimisation : public Univariate_Minimisation {
public:
  Sinc_Minimisation (Univariate_Minimisation_Config& P,
		     Annealing_Simple_Config& C,
		     Numbers_Generator& rnd) :
    Univariate_Minimisation(P, C, rnd) {}

  double energy_function	(void * configuration);
};

/* ------------------------------------------------------------ */

#endif

/* end of file */
