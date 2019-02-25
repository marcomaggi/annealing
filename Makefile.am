## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS		= -I meta/autotools
AUTOMAKE_OPTIONS	= foreign
EXTRA_DIST		= INSTALL meta/slackware/slack-desc.in
dist_doc_DATA		= README COPYING
AM_CFLAGS		= $(MMUX_CFLAGS)
AM_CPPFLAGS		= -I$(srcdir)/src

## --------------------------------------------------------------------

nodist_pkgconfig_DATA	= meta/scripts/annealing.pc


#### documentation

AM_MAKEINFOFLAGS	= --no-split
AM_MAKEINFOHTMLFLAGS	= --split=node -c WORDS_IN_PAGE=0 \
	-c PRE_BODY_CLOSE="<p>This document describes version <tt>$(PACKAGE_VERSION)</tt> of <em>$(PACKAGE_NAME)</em>.</p>"

info_TEXINFOS		= doc/annealing.texi
doc_annealing_TEXINFOS	= \
	doc/overview-linking.texi	\
	doc/macros.texi			\
	doc/gpl-3.0.texi		\
	doc/fdl-1.3.texi


#### libraries

annealing_CURRENT	= @annealing_VERSION_INTERFACE_CURRENT@
annealing_REVISION	= @annealing_VERSION_INTERFACE_REVISION@
annealing_AGE		= @annealing_VERSION_INTERFACE_AGE@

include_HEADERS			= src/annealing.h

lib_LTLIBRARIES			= libannealing.la
libannealing_la_CPPFLAGS	= $(AM_CPPFLAGS)
libannealing_la_CFLAGS		= $(AM_CFLAGS) $(GSL_CFLAGS)
libannealing_la_LDFLAGS		= -version-info $(annealing_CURRENT):$(annealing_REVISION):$(annealing_AGE) \
				  $(AM_LDFLAGS) $(GSL_LIBS)
libannealing_la_SOURCES	= \
	src/annealing-version.c		\
	src/annealing-basic.c		\
	src/annealing-manytries.c	\
	src/annealing-multibest.c	\
	src/annealing-internals.h	\
	src/annealing.h


#### tests

check_PROGRAMS	= \
	tests/version			\
	tests/fitting			\
	tests/many_sinc			\
	tests/multi_sinc		\
	tests/sinc			\
	tests/sinxy			\
	tests/tsp

TESTS				= $(check_PROGRAMS)

annealing_tests_cppflags	= $(AM_CPPFLAGS) -I$(srcdir)/tests
annealing_tests_cflags		= $(AM_CFLAGS) $(GSL_CFLAGS)
annealing_tests_ldadd		= $(AM_LDFLAGS) libannealing.la

## --------------------------------------------------------------------

tests_version_CPPFLAGS		= $(annealing_tests_cppflags)
tests_version_CFLAGS		= $(annealing_tests_cflags)
tests_version_LDADD		= $(annealing_tests_ldadd)

tests_fitting_CPPFLAGS		= $(annealing_tests_cppflags)
tests_fitting_CFLAGS		= $(annealing_tests_cflags)
tests_fitting_LDADD		= $(annealing_tests_ldadd)

tests_many_sinc_CPPFLAGS	= $(annealing_tests_cppflags)
tests_many_sinc_CFLAGS		= $(annealing_tests_cflags)
tests_many_sinc_LDADD		= $(annealing_tests_ldadd)

tests_multi_sinc_CPPFLAGS	= $(annealing_tests_cppflags)
tests_multi_sinc_CFLAGS		= $(annealing_tests_cflags)
tests_multi_sinc_LDADD		= $(annealing_tests_ldadd)

tests_sinc_CPPFLAGS		= $(annealing_tests_cppflags)
tests_sinc_CFLAGS		= $(annealing_tests_cflags)
tests_sinc_LDADD		= $(annealing_tests_ldadd)

tests_sinxy_CPPFLAGS		= $(annealing_tests_cppflags)
tests_sinxy_CFLAGS		= $(annealing_tests_cflags)
tests_sinxy_LDADD		= $(annealing_tests_ldadd)

tests_tsp_CPPFLAGS		= $(annealing_tests_cppflags)
tests_tsp_CFLAGS		= $(annealing_tests_cflags)
tests_tsp_LDADD			= $(annealing_tests_ldadd)

## --------------------------------------------------------------------

installcheck-local: $(TESTS)
	@for f in $(TESTS); do $(builddir)/$$f; done


#### common rules

include $(top_srcdir)/meta/automake/mmux-makefile-rules

### end of file
