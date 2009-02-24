# Part of: Annealing
# Contents: GNU Autoconf macros for Annealing loading
# Date: Sat Nov 24, 2007
# 
# Abstract
# 
# 
# 
# Copyright (c) 2007 Marco Maggi
# 
# This is free  software you can redistribute it  and/or modify it under
# the terms of  the GNU General Public License as  published by the Free
# Software Foundation; either  version 2, or (at your  option) any later
# version.
# 
# This  file is  distributed in  the hope  that it  will be  useful, but
# WITHOUT   ANY  WARRANTY;  without   even  the   implied  warranty   of
# MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
# General Public License for more details.
# 
# You  should have received  a copy  of the  GNU General  Public License
# along with this file; see the file COPYING.  If not, write to the Free
# Software Foundation,  Inc., 59  Temple Place -  Suite 330,  Boston, MA
# 02111-1307, USA.

AC_DEFUN([ANNEALING_LIBRARY], [
annealing_REQUESTED_MAJOR_VERSION=$1
annealing_REQUESTED_MINOR_VERSION=$2

AC_PATH_PROG([ANNEALING_CONFIG],[annealing-config],[:])
if test "${ANNEALING_CONFIG}" = ':' ; then
    AC_MSG_ERROR([missing Annealing Library (cannot find annealing-config)],1)
fi

annealing_AVAILABLE_MAJOR_VERSION=$("${ANNEALING_CONFIG}" --library-interface-major-version)
annealing_AVAILABLE_MINOR_VERSION=$("${ANNEALING_CONFIG}" --library-interface-minor-version)

if test -n "${annealing_REQUESTED_MAJOR_VERSION}" ; then
    AC_MSG_NOTICE([requested ANNEALING major version ${annealing_REQUESTED_MAJOR_VERSION}])

    if test "${annealing_REQUESTED_MAJOR_VERSION}" -ne "${annealing_AVAILABLE_MAJOR_VERSION}" ; then
        AC_MSG_ERROR([requested ANNEALING major version number ${annealing_REQUESTED_MAJOR_VERSION} while available is ${annealing_AVAILABLE_MAJOR_VERSION}],[1])
    fi
    if test -n "${annealing_REQUESTED_MINOR_VERSION}" ; then
        AC_MSG_NOTICE([requested ANNEALING minor version ${annealing_REQUESTED_MINOR_VERSION}])
        if test "${annealing_REQUESTED_MINOR_VERSION}" -gt "${annealing_AVAILABLE_MINOR_VERSION}" ; then
            AC_MSG_ERROR([requested ANNEALING minor version number ${annealing_REQUESTED_MINOR_VERSION} while available is ${annealing_AVAILABLE_MINOR_VERSION}],[1])
        fi
    fi
fi

ANNEALING_INCLUDEDIR=$("${ANNEALING_CONFIG}" --pkgincludedir)
AC_SUBST([ANNEALING_INCLUDEDIR],[${ANNEALING_INCLUDEDIR}])
AC_MSG_NOTICE([ANNEALING include directory ${ANNEALING_INCLUDEDIR}])

ANNEALING_CPPFLAGS="-I${ANNEALING_INCLUDEDIR}"
AC_SUBST([ANNEALING_CPPFLAGS],[${ANNEALING_CPPFLAGS}])

ANNEALING_LIBDIR=$("${ANNEALING_CONFIG}" --libdir)
AC_MSG_NOTICE([ANNEALING library directory ${ANNEALING_LIBDIR}])
ANNEALING_LDFLAGS="-L${ANNEALING_LIBDIR}"
AC_SUBST([ANNEALING_LDFLAGS],[${ANNEALING_LDFLAGS}])

ANNEALING_LIBRARY_ID=$("${ANNEALING_CONFIG}" --library-id)
AC_SUBST([ANNEALING_LIBRARY_ID],[${ANNEALING_LIBRARY_ID}])
AC_MSG_NOTICE([ANNEALING library identifier ${ANNEALING_LIBRARY_ID}])

# Verify the header file.
INCLUDES="${INCLUDES} -I${ANNEALING_INCLUDEDIR}"
annealing_saved_CPPFLAGS="${CPPFLAGS}"
CPPFLAGS="${CPPFLAGS} ${INCLUDES}"
AC_CHECK_HEADERS([annealing.h])
CPPFLAGS="${annealing_saved_CPPFLAGS}"

# Verify the static/shared library.
ANNEALING_LIBS=
AC_CHECK_LIB(${ANNEALING_LIBRARY_ID},annealing_version,[ANNEALING_LIBS="-l${ANNEALING_LIBRARY_ID}"],[
AC_MSG_ERROR([cannot link to Annealing Library])
])
AC_SUBST([ANNEALING_LIBS],[${ANNEALING_LIBS}])

])

### end of file
# Local Variables:
# mode: sh
# End:
