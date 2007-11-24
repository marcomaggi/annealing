/*
   Part of: annealing
   Contents: outer iterations computation
   Date: Sun Feb 25, 2007
   
   Abstract
   
	Compute the number of outer iterations.
   
   Copyright (c) 2007 Marco Maggi
   
   This is free  software you can redistribute it  and/or modify it under
   the terms of  the GNU General Public License as  published by the Free
   Software Foundation; either  version 2, or (at your  option) any later
   version.
   
   This  file is  distributed in  the hope  that it  will be  useful, but
   WITHOUT   ANY  WARRANTY;  without   even  the   implied  warranty   of
   MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
   General Public License for more details.
   
   You  should have received  a copy  of the  GNU General  Public License
   along with this file; see the file COPYING.  If not, write to the Free
   Software Foundation,  Inc., 59  Temple Place -  Suite 330,  Boston, MA
   02111-1307, USA.
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
