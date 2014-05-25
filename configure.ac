dnl @configure_input@
dnl

AC_INIT([Annealing],[0.5a0],[marco.maggi-ipsu@poste.it],
  [annealing],[http://github.com/marcomaggi/annealing/])
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

AX_PATH_GSL(1.16)

annealing_VERSION_INTERFACE_CURRENT=$2
annealing_VERSION_INTERFACE_REVISION=$3
annealing_VERSION_INTERFACE_AGE=$4
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
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

dnl end of file
