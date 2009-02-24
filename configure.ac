dnl @configure_input@
dnl

AC_INIT([annealing],[0.4.0])
AC_PREREQ(2.63)
AC_CONFIG_SRCDIR([./src/annealing.h])
DS_BEGIN
DS_DEFAULT_SECTIONS_DEV
DS_OPTIONS
DS_PROGRAMS
DS_DIRECTORIES
DS_PACKAGING
DS_COMPILED_STUFF
DS_C_LANGUAGE_SYSTEM_SETUP
DS_C_LANGUAGE
DS_C_LANGUAGE_LIBRARIES
DS_C_LANGUAGE_LIBRARY
DS_C_LANGUAGE_LIBRARY([annealing],[2],[0],[annealing])
AX_PATH_GSL(1.9)
AC_CONFIG_FILES([meta.d/annealing-config:meta/annealing-config.in])
AC_CONFIG_FILES([meta.d/annealing.pc:meta/annealing.pc.in])
AC_CONFIG_FILES([meta.d/slackware/bin/slack-desc:meta/slackware/bin/slack-desc.in])
AC_CONFIG_FILES([meta.d/slackware/doc/slack-desc:meta/slackware/doc/slack-desc.in])
AC_CONFIG_FILES([meta.d/slackware/dev/slack-desc:meta/slackware/dev/slack-desc.in])
DS_CONFIG_FILES
AC_OUTPUT

dnl end of file
