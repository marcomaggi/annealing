# configure.ac --
#
# Configuration file template for C-PROJECT a usage example for the
# Annealing extension to the GNU Scientific Library.

#page
## ------------------------------------------------------------
##  Setup.
##  ------------------------------------------------------------

AC_INIT([c-project], [1.0])
AC_PREREQ(2.60)
AC_CONFIG_SRCDIR([./tsp.c])

AC_SYS_INTERPRETER
AC_SYS_LARGEFILE
AC_SYS_LONG_FILE_NAMES
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

## ------------------------------------------------------------

#page
## ------------------------------------------------------------
## Common programs.
## ------------------------------------------------------------

AC_PROG_INSTALL
AC_PROG_MAKE_SET

## ------------------------------------------------------------

AC_PATH_PROG([MV],[mv],:)
AC_ARG_VAR([MV],[move files around])

AC_PATH_PROG([RM],[rm],:)
AC_ARG_VAR([RM],[remove files and directories])

AC_PATH_PROG([MKDIR],[mkdir],:)
AC_ARG_VAR([MKDIR],[create directories])

AC_PATH_PROG([RMDIR],[rmdir],:)
AC_ARG_VAR([RMDIR],[remove directories])

## ------------------------------------------------------------

#page
## ------------------------------------------------------------
## C language basic stuff.
## ------------------------------------------------------------

AC_CONFIG_HEADERS(config.h)

AC_DEFINE([_GNU_SOURCE],[1],[GNU libc symbol, see its documentation])

## ------------------------------------------------------------

AC_PROG_CC
AC_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_RANLIB
AC_PATH_PROG([AR],[ar],:)
AC_PATH_PROG([STRIP], [strip],:)
AC_PATH_PROG([GDB],[GDB],:)
AC_PROG_CC_C99

# Headers and C compiler features.
AC_INCLUDES_DEFAULT
AC_HEADER_STDC
AC_CHECK_HEADERS([assert.h])
AC_C_BIGENDIAN
AC_C_CHAR_UNSIGNED
AC_C_CONST
AC_C_FLEXIBLE_ARRAY_MEMBER
AC_C_INLINE
AC_C_TYPEOF
AC_C_RESTRICT
AC_C_VARARRAYS
AC_C_VOLATILE
AC_CHECK_TYPES([ptrdiff_t])
AC_TYPE_SIZE_T
AC_FUNC_MALLOC
#AC_CHECK_FUNCS([memmove memset strerror strchr])

AC_SUBST([NO_MINUS_C_MINUS_O])
AC_ARG_VAR([GNU_C_FLAGS],[fixed GNU C compiler flags])

## ------------------------------------------------------------

AC_ARG_VAR([INCLUDES],[directory options for include files])

# Note: 'DEFS' substitution is automatically handled.
# Note: 'CPPFLAGS' substitution is automatically handled.

## ------------------------------------------------------------

AC_SUBST([LIBS])
AC_SUBST([LDFLAGS_RPATH],['-Wl,-rpath,$(libdir)'])
AC_ARG_VAR([LDFLAGS],[fixed linker flags])

# AC_CHECK_LIB([dl],[dlopen],[LDFLAGS_DL=-ldl])
# AC_SUBST(LDFLAGS_DL)

## ------------------------------------------------------------

AC_CACHE_SAVE

## ------------------------------------------------------------

#page
## ------------------------------------------------------------
## GSL and Annealing.
## ------------------------------------------------------------

AX_PATH_GSL(1.9)
ANNEALING_LIBRARY(1)

## ------------------------------------------------------------

#page
## ------------------------------------------------------------
## Done.
## ------------------------------------------------------------

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

### end of file
# Local Variables:
# mode: sh
# End:
