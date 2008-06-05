/*
   Part of: annealing
   Contents: minimisation of sinc(t)
   Date: Thu Feb 22, 2007
   
   Abstract
   
	Find the minimum of 'f(t) = -sin(t)/t'.
   
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
 ** Headers.
 ** ----------------------------------------------------------*/

#include "internal.hpp"
#include "sinc.hpp"

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Main.
 ** ----------------------------------------------------------*/

int
main (int argc, char ** argv)
{
  int			verbose_mode = 0;
  Sinc_Minimisation	A = Sinc_Minimisation(100.0);

  {
    int c;

    while ((c = getopt(argc, argv, "hv")) != -1)
      switch (c)
	{
	case 'h':
	  fprintf(stderr, "usage: test_sinc [-v] [-h]\n");
	  exit(EXIT_SUCCESS);
	case 'v':
	  verbose_mode = 1;
	  break;
	default:
	  fprintf(stderr, "test_sinc error: unknown option %c\n", c);
	  exit(EXIT_FAILURE);
	}
  }

  std::cout << std::endl
	    << "------------------------------------------------------------"
	    << std::endl
	    << "test_sinc: sinc minimisation with simulated annealing"
	    << std::endl;

  A.solve();

  std::cout << "test_sinc: final best solution: " << A.best_estimation()
	    << ", global 0.0"
	    << std::endl
	    << "------------------------------------------------------------"
	    << std::endl << std::endl;

  exit(EXIT_SUCCESS);
}

/* end of file */
