/*
   Part of: annealing
   Contents: methods definitions for sinc minimisation.
   Date: Thu Feb 22, 2007
   
   Abstract
   
	Defines the methods required  to find the minimum of
	'f(t) = -sin(t)/t'.
   
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
#include "sinc.hpp"

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Methods.
 ** ----------------------------------------------------------*/

double
Sinc_Minimisation::energy_function (void * configuration)
{
  double	C = *((double *)configuration);

  return -sin(C)/C;
}


/* end of file */
