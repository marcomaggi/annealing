/*
   Part of: annealing
   Contents: outer iterations computation
   Date: Sun Feb 25, 2007

   Abstract

	Compute the number of outer iterations.

   Copyright (c) 2007, 2009 Marco Maggi <marcomaggi@gna.org>

   This program is free software:  you can redistribute it and/or modify
   it under the terms of the  GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or (at
   your option) any later version.

   This program is  distributed in the hope that it  will be useful, but
   WITHOUT  ANY   WARRANTY;  without   even  the  implied   warranty  of
   MERCHANTABILITY  or FITNESS FOR  A PARTICULAR  PURPOSE.  See  the GNU
   General Public License for more details.

   You should  have received  a copy of  the GNU General  Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>


/** ------------------------------------------------------------
 ** Main.
 ** ----------------------------------------------------------*/

int
main (void)
{
  double initial = 100.0;
  double minimum = 1.0;
  double damping = 1.005;
  size_t count   = 1;
  double l;

  for (l = initial; l >= minimum; l /= damping, ++count)
    ;
  printf("count from %.1f to %.1f with damping %.3f: %u\n",
	 initial, minimum, damping, count);
  exit(EXIT_SUCCESS);
}


/* end of file */
