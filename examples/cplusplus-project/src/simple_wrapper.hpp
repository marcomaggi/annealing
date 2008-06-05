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

#include <iostream>
#include <cmath>

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

class Annealing_Simple {
protected:
  annealing_simple_workspace_t *	S;

public:
  virtual double energy_function(void * configuration);
  virtual void	step_function	(void * configuration);

  virtual void	log_function	(void);
  virtual void	copy_function	(void * dst_configuration,
				 void * src_configuration);
public:
  Annealing_Simple (gsl_rng * numbers_generator);
  Annealing_Simple (void);
  ~Annealing_Simple (void);

  void		solve (void);

private:
  void initialisation (gsl_rng * numbers_generator);
};

/* ------------------------------------------------------------ */

#endif

/* end of file */
