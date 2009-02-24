dnl @configure_input@
dnl

dnl page
dnl --------------------------------------------------------------------
dnl Setup.
dnl --------------------------------------------------------------------

AC_INIT([annealing],[0.3.0])
AC_PREREQ(2.63)
AC_CONFIG_SRCDIR([./src/annealing.h])

DS_BEGIN
DS_INCLUDE_SECTION([BIN_RULES],[yes])
DS_INCLUDE_SECTION([DOC_RULES],[yes])
DS_INCLUDE_SECTION([DEV_RULES],[yes])
DS_INCLUDE_SECTION([AUTOCONF_DIRS],[yes])
DS_INCLUDE_SECTION([DEVELSTUFF_DIRS],[yes])
DS_OPTIONS
DS_PROGRAMS
DS_DIRECTORIES
DS_PACKAGING
DS_COMPILED_STUFF
DS_C_LANGUAGE_SYSTEM_SETUP
DS_C_LANGUAGE
DS_C_LANGUAGE_LIBRARIES
DS_C_LANGUAGE_LIBRARY
DS_C_LANGUAGE_LIBRARY([annealing],[1],[0])

AX_PATH_GSL(1.9)
# AC_CHECK_HEADER([gsl/gsl_errno.h],,[AC_MSG_ERROR([unable to find header],1)])
# AC_CHECK_HEADER([gsl/gsl_vector.h],,[AC_MSG_ERROR([unable to find header],1)])
# AC_CHECK_HEADER([gsl/gsl_matrix.h],,[AC_MSG_ERROR([unable to find header],1)])
# AC_CHECK_LIB(m,main,[],[AC_MSG_ERROR([unable to find library],1)])
# AC_CHECK_LIB(gslcblas,main,[],[AC_MSG_ERROR([unable to find library],1)])
# AC_CHECK_LIB(gsl,main,[],[AC_MSG_ERROR([unable to find library],1)])

dnl page
dnl --------------------------------------------------------------------
dnl Done.
dnl --------------------------------------------------------------------

AC_CONFIG_FILES([meta.d/annealing-config:meta/annealing-config.in])
AC_CONFIG_FILES([meta.d/annealing.pc:meta/annealing.pc.in])
AC_CONFIG_FILES([meta.d/slackware/bin/slack-desc:meta/slackware/bin/slack-desc.in])
AC_CONFIG_FILES([meta.d/slackware/doc/slack-desc:meta/slackware/doc/slack-desc.in])
AC_CONFIG_FILES([meta.d/slackware/dev/slack-desc:meta/slackware/dev/slack-desc.in])
DS_CONFIG_FILES
AC_OUTPUT

dnl end of file
