## Process this file with automake to produce Makefile.in

ACLOCAL_MFLAGS		= -I autotools
AUTOMAKE_OPTIONS	= foreign
EXTRA_DIST		= INSTALL
dist_doc_DATA		= README COPYING

bin_PROGRAMS		= program
program_SOURCES		= \
	src/internal.hpp			\
	src/main.cpp				\
	src/numbers_generator.cpp		\
	src/numbers_generator.hpp		\
	src/simple_wrapper.cpp			\
	src/simple_wrapper.hpp			\
	src/sinc.cpp				\
	src/sinc.hpp				\
	src/univariate_minimisation.cpp		\
	src/univariate_minimisation.hpp

check_PROGRAMS		= program

TESTS			= $(check_PROGRAMS)

### end of file
