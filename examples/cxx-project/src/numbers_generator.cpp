/*
   Part of: annealing
   Contents: random numbers generator wrapper
   Date: Fri Jun  6, 2008
   
   Abstract
   
	Defines the methods for the wrapper class of GSL's
	numbers generator.
   
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


/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#include "numbers_generator.hpp"
#include <new>

using std::bad_alloc;

/* ------------------------------------------------------------ */

#ifndef NUMBERS_GENERATOR_DEFAULT_SEED
#  define NUMBERS_GENERATOR_DEFAULT_SEED	15
#endif

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Constructors.
 ** ----------------------------------------------------------*/

void
Numbers_Generator::initialise (const gsl_rng_type * type, unsigned long seed)
  throw(bad_alloc)
{
  pointer = gsl_rng_alloc(type);
  if (NULL == pointer)
    {
      throw(bad_alloc());
    }
  gsl_rng_set(pointer, seed);
}

/* ------------------------------------------------------------ */

Numbers_Generator::Numbers_Generator (const gsl_rng_type * type, unsigned long seed)
{
  initialise(type, seed);
}
Numbers_Generator::Numbers_Generator (const gsl_rng_type * type)
{
  initialise(type, NUMBERS_GENERATOR_DEFAULT_SEED);
}
Numbers_Generator::Numbers_Generator (unsigned long seed)
{
  initialise(gsl_rng_rand, seed);
}
Numbers_Generator::Numbers_Generator (void)
{
  initialise(gsl_rng_rand, NUMBERS_GENERATOR_DEFAULT_SEED);
}
Numbers_Generator::~Numbers_Generator (void)
{
  gsl_rng_free(pointer);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Methods.
 ** ----------------------------------------------------------*/

unsigned long
Numbers_Generator::sample_integer (void)
{
  return gsl_rng_get(pointer);
}
unsigned long
Numbers_Generator::sample_integer_in_range (unsigned long exclusive_upper_limit)
{
  return gsl_rng_uniform_int(pointer, exclusive_upper_limit);
}
unsigned long
Numbers_Generator::maximum_integer (void)
{
  return gsl_rng_max(pointer);
}
unsigned long
Numbers_Generator::minimum_integer (void)
{
  return gsl_rng_max(pointer);
}

/* ------------------------------------------------------------ */

double
Numbers_Generator::sample (void)
{
  return gsl_rng_uniform(pointer);
}
double
Numbers_Generator::sample_positive (void)
{
  return gsl_rng_uniform_pos(pointer);
}



/* end of file */
