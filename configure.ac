# configure.ac --
#

AC_INIT([guile-endian], [0.1.0], [marcomaggi@gna.org])
AC_PREREQ(2.57)
AC_COPYRIGHT([Copyright (C) 2007 by Marco Maggi])
AC_CONFIG_SRCDIR([./annealing.c])


#page
## ------------------------------------------------------------
## Common programs.
## ------------------------------------------------------------

AC_PATH_PROG([MKDIR],[mkdir],[false])
AC_ARG_VAR([MKDIR],[creates directories recursively])

AC_PATH_PROG([RM],[rm],[false])
AC_ARG_VAR([RM],[deletes files and directories recursively])

AC_PATH_PROG([TAR],[tar],[false])
AC_ARG_VAR([TAR],[the GNU tar program])

AC_PATH_PROG([GUILE],[guile],[false])
AC_ARG_VAR([GUILE],[the Guile interpreter])

AC_PATH_PROG([FIND],[find],[false])
AC_PATH_PROG([GUILE_SNARF],[guile-snarf],[false])

AC_PATH_PROG([M4],[m4],[false])

AC_PROG_INSTALL
AC_PROG_MAKE_SET

#page
## ------------------------------------------------------------
## C language environment.
## ------------------------------------------------------------

AC_CONFIG_HEADERS(config.h)

AC_ARG_ENABLE([assert],
    AC_HELP_STRING([--disable-assert],[disable assertions]),[
        if test "$enableval" = yes ; then
            ENABLED_ASSERTIONS=1
        else
            ENABLED_ASSERTIONS=0
        fi],[ENABLED_ASSERTIONS=1])
AC_DEFINE_UNQUOTED([ENABLED_ASSERTIONS],$ENABLED_ASSERTIONS,[turns off assertions])

AC_DEFINE([_GNU_SOURCE], [1], [GNU libc symbol, see its documentation])

AC_PROG_CC
AC_PROG_CC_C_O
AC_PROG_CPP
AC_PATH_PROG([AR], [ar], :)
AC_PATH_PROG([STRIP], [strip], :)
AC_PROG_RANLIB
AC_C_BIGENDIAN
AC_C_CONST
AC_C_INLINE
AC_FUNC_MALLOC
AC_HEADER_STDC
AC_TYPE_SIZE_T
AC_CHECK_HEADERS([limits.h])
LINKFLAGS="-Wl,-rpath,$libdir"
AC_ARG_VAR([LINKFLAGS], [fixed linker flags])
AC_ARG_VAR([INCLUDES], [paths for include files])
AC_SUBST(LIBS)
AC_SUBST(INCLUDES)

AC_CACHE_SAVE


#page
## ------------------------------------------------------------
## GSL.
## ------------------------------------------------------------

AC_CHECK_HEADER([gsl/gsl_errno.h],,[AC_MSG_ERROR([unable to find header],1)])
AC_CHECK_HEADER([gsl/gsl_vector.h],,[AC_MSG_ERROR([unable to find header],1)])
AC_CHECK_HEADER([gsl/gsl_matrix.h],,[AC_MSG_ERROR([unable to find header],1)])


AC_CHECK_LIB(m,main,[],[AC_MSG_ERROR([unable to find library],1)])
AC_CHECK_LIB(gslcblas,main,[],[AC_MSG_ERROR([unable to find library],1)])
AC_CHECK_LIB(gsl,main,[],[AC_MSG_ERROR([unable to find library],1)])


#page
## ------------------------------------------------------------
## Let's go.
## ------------------------------------------------------------

AC_CONFIG_FILES([Makefile])
AC_OUTPUT()


### end of file
# Local Variables:
# mode: sh
# End:
