/*
   
   Part of: Annealing
   Contents: miscellaneous functions
   Date: Thu Jun  5, 2008
   
   Abstract
   
   
   
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

#include "internal.h"
#include "annealing.h"

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Version functions.
 ** ----------------------------------------------------------*/

const char *
annealing_version (void)
{
  return PACKAGE_XVERSION;
}
int
annealing_library_major_version (void)
{
  return annealing_INTERFACE_MAJOR_VERSION;
}
int
annealing_library_minor_version (void)
{
  return annealing_INTERFACE_MINOR_VERSION;
}


/* end of file */
