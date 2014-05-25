## Process this file with automake to produce Makefile.in

ACLOCAL_MFLAGS		= -I autotools
AUTOMAKE_OPTIONS	= foreign
EXTRA_DIST		= INSTALL
dist_doc_DATA		= README COPYING

#page
#### documentation

AM_MAKEINFOFLAGS	= --no-split

info_TEXINFOS		= doc/annealing.texi
doc_annealing_TEXINFOS	= \
	doc/fdl-1.3.texi		\
	doc/gpl-3.0.texi

#page
#### library

annealing_CURRENT	= @annealing_VERSION_INTERFACE_CURRENT@
annealing_REVISION	= @annealing_VERSION_INTERFACE_REVISION@
annealing_AGE		= @annealing_VERSION_INTERFACE_AGE@
annealing_VERSION	= $(annealing_CURRENT):$(annealing_REVISION):$(annealing_AGE)

lib_LTLIBRARIES			= libannealing.la
libannealing_la_LDFLAGS		= -version-info $(annealing_VERSION)
libannealing_la_SOURCES		= \
	src/misc.c			\
	src/basic.c			\
	src/manytries.c			\
	src/multibest.c			\
	src/internal.h			\
	src/annealing.h

include_HEADERS			= src/annealing.h

#page
#### interface to "make check"

# Read "Parallel Test Harness" in the documentation of GNU Automake to
# understand how to use this interface for "make check".
#
# We must make sure that this ends with a semicolon!!!
#AM_TESTS_ENVIRONMENT	=

tests_fitting_CPPFLAGS		= -I$(top_srcdir)/src
tests_many_sinc_CPPFLAGS	= -I$(top_srcdir)/src
tests_multi_sinc_CPPFLAGS	= -I$(top_srcdir)/src
tests_sinc_CPPFLAGS		= -I$(top_srcdir)/src
tests_sinxy_CPPFLAGS		= -I$(top_srcdir)/src
tests_tsp_CPPFLAGS		= -I$(top_srcdir)/src

tests_fitting_LDADD		= libannealing.la
tests_many_sinc_LDADD		= libannealing.la
tests_multi_sinc_LDADD		= libannealing.la
tests_sinc_LDADD		= libannealing.la
tests_sinxy_LDADD		= libannealing.la
tests_tsp_LDADD			= libannealing.la

check_PROGRAMS		= \
	tests/fitting			\
	tests/many_sinc			\
	tests/multi_sinc		\
	tests/sinc			\
	tests/sinxy			\
	tests/tsp

TESTS			= $(check_PROGRAMS)

### end of file
