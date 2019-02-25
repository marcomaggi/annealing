# @configure_input@
#

AC_PREREQ([2.69])
MMUX_PKG_VERSIONS([0],[6],[0],[devel.0])
AC_INIT([Annealing],[MMUX_PACKAGE_VERSION],
  [marco.maggi-ipsu@poste.it],
  [annealing],[http://github.com/marcomaggi/annealing])
MMUX_INIT
MMUX_LIBTOOL_LIBRARY_VERSIONS([annealing],2,0,0)
AC_REVISION([0.1])
AC_COPYRIGHT([Copyright (c) 2016-2019 Marco Maggi <marco.maggi-ipsu@poste.it>

This program is free software: you  can redistribute it and/or modify it
under the  terms of the GNU  General Public License as  published by the
Free Software Foundation,  either version 3 of the License,  or (at your
option) any later version.

This program  is distributed  in the  hope that it  will be  useful, but
WITHOUT   ANY   WARRANTY;  without   even   the   implied  warranty   of
MERCHANTABILITY  or  FITNESS FOR  A  PARTICULAR  PURPOSE.  See  the  GNU
General Public License for more details.

You should have received a copy  of the GNU General Public License along
with this program.  If not, see <http://www.gnu.org/licenses/>.
])
AC_CONFIG_SRCDIR([src/])
AC_CONFIG_MACRO_DIR([meta/autotools])
AC_CONFIG_AUX_DIR([meta/autotools])
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([1.16 foreign subdir-objects])
AM_MAINTAINER_MODE

AM_PROG_AR
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P

LT_PREREQ([2.4])
LT_INIT


#### basic system inspection

MMUX_CHECK_TARGET_OS
AX_IS_RELEASE([git-directory])

AC_LANG([C])
MMUX_LANG_C11

AM_PROG_AS
AM_PROG_CC_C_O
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([assert.h stdint.h strings.h errno.h stddef.h unistd.h])

AC_CACHE_SAVE


#### checks for library functions

dnl AC_FUNC_MALLOC
dnl AC_FUNC_MEMCMP
dnl AC_CHECK_FUNCS([memmove memset strerror])
dnl AC_CHECK_FUNCS([mmap munmap msync mremap madvise mlock munlock mlockall munlockall mprotect])
dnl AC_CHECK_FUNCS([getenv])

dnl AC_CACHE_SAVE


#### external libraries

PKG_PREREQ([0.29])
PKG_INSTALLDIR
MMUX_CHECK_PKG_CONFIG_MACROS
PKG_CHECK_MODULES([GSL],[gsl >= 2.5])

AC_CACHE_SAVE


#### finish

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile]
   [meta/scripts/annealing.pc]
   [meta/slackware/slack-desc])
MMUX_OUTPUT
AC_OUTPUT

### end of file
