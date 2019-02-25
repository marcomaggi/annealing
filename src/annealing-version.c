/*
   Part of: Annealing
   Contents: version functions
   Date: Thu Jun  5, 2008

   Abstract



   Copyright (c) 2008, 2009, 2014, 2019 Marco Maggi
   <marco.maggi-ipsu@poste.it>

   This program is free software: you can  redistribute it and/or modify it under the
   terms  of  the GNU  General  Public  License as  published  by  the Free  Software
   Foundation,  either version  3  of the  License,  or (at  your  option) any  later
   version.

   This program is  distributed in the hope  that it will be useful,  but WITHOUT ANY
   WARRANTY; without  even the implied warranty  of MERCHANTABILITY or FITNESS  FOR A
   PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   You should have received a copy of  the GNU General Public License along with this
   program.  If not, see <http://www.gnu.org/licenses/>.
*/


/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#include "annealing-internals.h"


/** ------------------------------------------------------------
 ** Version functions.
 ** ----------------------------------------------------------*/

int
annealing_version_interface_current (void)
{
  return annealing_VERSION_INTERFACE_CURRENT;
}
int
annealing_version_interface_revision (void)
{
  return annealing_VERSION_INTERFACE_REVISION;
}
int
annealing_version_interface_age (void)
{
  return annealing_VERSION_INTERFACE_AGE;
}
const char *
annealing_version_string (void)
{
  return annealing_VERSION_INTERFACE_STRING;
}

/* end of file */
