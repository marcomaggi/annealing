/*
   Part of: annealing
   Contents: random numbers generator wrapper
   Date: Fri Jun  6, 2008
   
   Abstract
   
	Declares a wrapper for GSL's numbers generator.
   
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

#ifndef NUMBERS_GENERATOR_HPP
#define NUMBERS_GENERATOR_HPP


/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#include <new>

using std::bad_alloc;

extern "C" {

#include <gsl/gsl_rng.h>

}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Class.
 ** ----------------------------------------------------------*/

class Numbers_Generator
{
private:
  void initialise (const gsl_rng_type * type, unsigned long seed)
    throw(bad_alloc);

protected:
  gsl_rng *	pointer;

public:
  Numbers_Generator (const gsl_rng_type * type, unsigned long seed);
  Numbers_Generator (const gsl_rng_type * type);
  Numbers_Generator (unsigned long seed);
  Numbers_Generator (void);
  ~Numbers_Generator (void);

  gsl_rng * get_pointer (void) { return pointer; }

  unsigned long	sample_integer		(void);
  unsigned long	sample_integer_in_range	(unsigned long exclusive_upper_limit);

  unsigned long	maximum_integer (void);
  unsigned long	minimum_integer (void);

  double	sample		(void);
  double	sample_positive	(void);
};

#endif

/* end of file */
