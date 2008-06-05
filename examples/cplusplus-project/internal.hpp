/*
   Part of: annealing
   Contents: internal header file
   Date: Thu Jun  5, 2008
   
   Abstract
   
	This header must be included in all the source file.
   
   Copyright (c) 2008 Marco Maggi
   
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

#ifndef INTERNAL_HPP
#define INTERNAL_HPP

#include <iostream>
#include <cmath>

extern "C" {

#include <float.h>		/* for 'DBL_MIN' */
#include <unistd.h>		/* for 'getopt()' */
#include <gsl/gsl_rng.h>
#include "annealing.h"

}

#endif

/* end of file */
