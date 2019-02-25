/*
  Part of: Annealing
  Contents: test for version functions
  Date: Feb 25, 2019

  Abstract

	Test file for version functions.

  Copyright (C) 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

  See the COPYING file.
*/

#include <annealing.h>
#include <stdio.h>
#include <stdlib.h>

int
main (void)
{
  printf("version number string: %s\n", annealing_version_string());
  printf("libtool version number: %d:%d:%d\n",
	 annealing_version_interface_current(),
	 annealing_version_interface_revision(),
	 annealing_version_interface_age());
  exit(EXIT_SUCCESS);
}

/* end of file */
