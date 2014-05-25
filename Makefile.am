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
libannealing_la_SOURCES	= \
	src/internal.h
	src/misc.c
	src/basic.c
	src/manytries.c
	src/multibest.c

libannealing_DEPENDENCIES	= src/annealing.h
include_HEADERS			= src/annealing.h


### end of file
