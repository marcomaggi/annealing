dnl @configure_input@
dnl

AC_INIT([CXXProject],[0.1.0],[marco.maggi-ipsu@poste.it],[cxxproject])
AC_CONFIG_SRCDIR([src/])
AC_CONFIG_MACRO_DIR([autotools])
AC_CONFIG_AUX_DIR([autotools])
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([-Wall foreign dist-xz no-dist-gzip subdir-objects])
AM_MAINTAINER_MODE

AM_PROG_AR
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P

LT_PREREQ([2.4])
LT_INIT

AC_LANG([C])
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CC_STDC

dnl Checks for header files.
AC_HEADER_STDC

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

dnl --------------------------------------------------------------------

PKG_CHECK_MODULES([GSL],[gsl >= 1.16],[gsl=yes],[gsl=no])

GSL_CPPFLAGS=$(pkg-config gsl --cflags)
GSL_LDFLAGS=$(pkg-config gsl --libs)

AS_IF([test "x$gsl" = "xyes"],
  [AC_DEFINE([HAVE_GSL_H],[1],[true if GSL header file is present])
   CPPFLAGS="$CPPFLAGS $GSL_CPPFLAGS"
   LIBS="$LIBS $GSL_LDFLAGS"],
  [AC_MSG_FAILURE([unable to find required version of GSL],[1])])

dnl --------------------------------------------------------------------

PKG_CHECK_MODULES([Annealing],[annealing >= 0.5],[annealing=yes],[annealing=no])

ANNEALING_CPPFLAGS=$(pkg-config annealing --cflags)
ANNEALING_LDFLAGS=$(pkg-config annealing --libs)

AS_IF([test "x$annealing" = "xyes"],
  [AC_DEFINE([HAVE_ANNEALING_H],[1],[true if annealing header file is present])
   CPPFLAGS="$CPPFLAGS $ANNEALING_CPPFLAGS"
   LIBS="$LIBS $ANNEALING_LDFLAGS"],
  [AC_MSG_FAILURE([unable to find required version of ANNEALING],[1])])

dnl --------------------------------------------------------------------

AC_LANG([C++])
AC_PROG_CXX
dnl AM_PROG_CC_C_O
dnl AC_PROG_CC_STDC

dnl --------------------------------------------------------------------

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

dnl end of file
