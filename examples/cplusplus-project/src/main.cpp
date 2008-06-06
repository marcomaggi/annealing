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

using namespace std;


/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Main.
 ** ----------------------------------------------------------*/

int
main (int argc, char ** argv)
{
  int	verbose_mode = 0;

  {
    int c;

    while ((c = getopt(argc, argv, "hv")) != -1)
      switch (c)
	{
	case 'h':
	  cerr << "usage: sinc [-v] [-h]" << endl;
	  exit(EXIT_SUCCESS);
	case 'v':
	  verbose_mode = 1;
	  break;
	default:
	  cerr << "sinc error: unknown option " << c << endl;
	  exit(EXIT_FAILURE);
	}
  }

  cout << endl
       << "------------------------------------------------------------"
       << endl
       << "test_sinc: sinc minimisation with simulated annealing"
       << endl;

  {
    Sinc_Minimisation	A(100.0);
      

    A.solve();

    cout << "test_sinc: final best solution: " << A.best_estimation()
	 << ", global 0.0"
	 << endl
	 << "------------------------------------------------------------"
	 << endl << endl;
  }
  
  exit(EXIT_SUCCESS);
}

/* end of file */
