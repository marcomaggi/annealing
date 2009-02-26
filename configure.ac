dnl @configure_input@
dnl

AC_INIT([annealing],[0.4a0])
AC_PREREQ(2.63)
AC_CONFIG_SRCDIR([./src/annealing.h])
DS_C99_LANGUAGE_COMMON_SETUP
DS_C_LANGUAGE_LIBRARY([annealing],1,1,[annealing])
AX_PATH_GSL(1.9)
AC_CONFIG_FILES([meta.d/annealing-config:meta/annealing-config.in])
AC_CONFIG_FILES([meta.d/annealing.pc:meta/annealing.pc.in])
AC_CONFIG_FILES([meta.d/slackware/bin/slack-desc:meta/slackware/bin/slack-desc.in])
AC_CONFIG_FILES([meta.d/slackware/doc/slack-desc:meta/slackware/doc/slack-desc.in])
AC_CONFIG_FILES([meta.d/slackware/dev/slack-desc:meta/slackware/dev/slack-desc.in])
DS_CONFIG_FILES
AC_OUTPUT

dnl end of file
