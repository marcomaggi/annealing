dnl @configure_input@
dnl

AC_INIT([Annealing],[0.5a0],[marco.maggi-ipsu@poste.it],[annealing],[http://github.com/marcomaggi/annealing/])
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

PKG_CHECK_MODULES([GSL],[gsl >= 1.16],[gsl=yes],[gsl=no])

GSL_CPPFLAGS=$(pkg-config gsl --cflags)
GSL_LDFLAGS=$(pkg-config gsl --libs)

AS_IF([test "x$gsl" = "xyes"],
  [AC_DEFINE([HAVE_GSL_H],[1],[true if GSL header file is present])
   CPPFLAGS="$CPPFLAGS $GSL_CPPFLAGS"
   LIBS="$LIBS $GSL_LDFLAGS"],
  [AC_MSG_FAILURE([unable to find required version of GSL],[1])])

annealing_VERSION_INTERFACE_CURRENT=1
annealing_VERSION_INTERFACE_REVISION=1
annealing_VERSION_INTERFACE_AGE=1
AC_DEFINE_UNQUOTED([annealing_VERSION_INTERFACE_CURRENT],
  [$annealing_VERSION_INTERFACE_CURRENT],
  [current interface number])
AC_DEFINE_UNQUOTED([annealing_VERSION_INTERFACE_REVISION],
  [$annealing_VERSION_INTERFACE_REVISION],
  [current interface implementation number])
AC_DEFINE_UNQUOTED([annealing_VERSION_INTERFACE_AGE],
  [$annealing_VERSION_INTERFACE_AGE],
  [current interface age number])
AC_DEFINE_UNQUOTED([annealing_VERSION_INTERFACE_STRING],
  ["$annealing_VERSION_INTERFACE_CURRENT.$annealing_VERSION_INTERFACE_REVISION"],
  [library interface version])
AC_SUBST([annealing_VERSION_INTERFACE_CURRENT])
AC_SUBST([annealing_VERSION_INTERFACE_REVISION])
AC_SUBST([annealing_VERSION_INTERFACE_AGE])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile]
  [meta/annealing.pc])
AC_OUTPUT

dnl end of file
