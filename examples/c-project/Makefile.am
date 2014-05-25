## Process this file with automake to produce Makefile.in

ACLOCAL_MFLAGS		= -I autotools
AUTOMAKE_OPTIONS	= foreign
EXTRA_DIST		= INSTALL
dist_doc_DATA		= README COPYING

bin_PROGRAMS		= \
	src/fitting		\
	src/many_sinc		\
	src/multi_sinc		\
	src/sinc		\
	src/sinxy		\
	src/tsp

check_PROGRAMS			= \
	src/fitting		\
	src/many_sinc		\
	src/multi_sinc		\
	src/sinc		\
	src/sinxy		\
	src/tsp

TESTS				= $(check_PROGRAMS)

### end of file
