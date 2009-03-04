# Makefile.in --


#page
## ------------------------------------------------------------
## Directories.
## ------------------------------------------------------------

srcdir		= ..
builddir	= .

## ------------------------------------------------------------

#page
## ------------------------------------------------------------
## Programs.
## ------------------------------------------------------------

SHELL		= @SHELL@
@SET_MAKE@

MKDIR		= @MKDIR@ --parents --verbose
MV		= @MV@ --verbose --
RM		= @RM@ --force --recursive --verbose --
RM_FILE		= @RM@ --force --verbose --
RM_SILENT	= @RM@ --force --recursive --
RMDIR		= @RMDIR@ --parents --ignore-fail-on-non-empty --

## ------------------------------------------------------------

#page
## ------------------------------------------------------------
## C language stuff.
## ------------------------------------------------------------

CC		= @CC@
CPP		= @CPP@
AR		= @AR@ rc
RANLIB		= @RANLIB@
STRIP		= @STRIP@
GDB		= @GDB@

## ------------------------------------------------------------

# Notice that '-std=c99' is appended to 'CC' by the Autoconf
# macro 'AC_PROG_CC_C99'.
C_DEFAULT	?= -pipe -fPIC
C_WARNINGS	?= -Wall -W -Wextra -pedantic \
			-Wmissing-prototypes \
			-Wpointer-arith -Wcast-qual -Wcast-align \
			-Wwrite-strings -Wnested-externs \
			-Wstrict-prototypes -Wshadow

## ------------------------------------------------------------
## Preprocessor stuff.

DEFS		= @DEFS@
INCLUDES	= @INCLUDES@ -I. -I$(srcdir)
CPPFLAGS	= @CPPFLAGS@ $(DEFS) $(INCLUDES)

## ------------------------------------------------------------
## Linker stuff.

LIBS		= @LIBS@
LDFLAGS		= @LDFLAGS@
LDFLAGS_RPATH	= @LDFLAGS_RPATH@
LDFLAGS_DL	= @LDFLAGS_DL@

## ------------------------------------------------------------

GNU_C_FLAGS	= @GNU_C_FLAGS@ $(C_DEFAULT) $(C_WARNINGS)
ALL_CFLAGS	= $(GNU_C_FLAGS) $(CPPFLAGS)
CFLAGS		= @CFLAGS@
OBJEXT		= @OBJEXT@

## ------------------------------------------------------------

NO_MINUS_C_MINUS_O	= @NO_MINUS_C_MINUS_O@
ifeq ($(strip $(NO_MINUS_C_MINUS_O)),)
CC_COMPILE_OUTPUT	= -c -o
else
CC_COMPILE_OUTPUT	= -o
endif

CC_COMMAND_LINE		= $(CC) $(ALL_CFLAGS) $(CFLAGS)
CC_COMPILE		= $(CC_COMMAND_LINE) $(CC_COMPILE_OUTPUT)
CC_BUILD_PROGRAM	= $(CC_COMMAND_LINE) $(LDFLAGS) $(LIBS) -o
CC_BUILD_LIBRARY	= $(CC_COMMAND_LINE) -shared $(LDFLAGS) $(LIBS) -o

## ------------------------------------------------------------
## Project specific variables.

GSL_CFLAGS		= @GSL_CFLAGS@
GSL_LIBS		= @GSL_LIBS@

ANNEALING_INCLUDEDIR	= @ANNEALING_INCLUDEDIR@
ANNEALING_LDFLAGS	= @ANNEALING_LDFLAGS@
ANNEALING_LIBS		= @ANNEALING_LIBS@

INCLUDES	+= -I$(ANNEALING_INCLUDEDIR)
CPPFLAGS	+= $(GSL_CFLAGS)
LDFLAGS		+= $(ANNEALING_LDFLAGS)
LIBS		+= $(GSL_LIBS) $(ANNEALING_LIBS)

## ------------------------------------------------------------

#page
## ------------------------------------------------------------
## Rules.
## ------------------------------------------------------------

SOURCES		= $(wildcard $(srcdir)/*.c)
OBJECTS		= $(notdir $(SOURCES:.c=.$(OBJEXT)))
TARGETS		= $(OBJECTS:.$(OBJEXT)=)
CLEANFILES	= $(TARGETS) $(OBJECTS)

.PHONY: all clean test test-verbose

all: $(TARGETS)

clean:
	-$(RM) $(CLEANFILES)

test: all
	$(foreach f,$(TARGETS), ./$(f);)

test-verbose: all
	$(foreach f,$(TARGETS), ./$(f) -v;)

## ------------------------------------------------------------

vpath	%.h		$(srcdir)
vpath	%.$(OBJEXT)	$(builddir)

$(OBJECTS) : %.$(OBJEXT) : $(srcdir)/%.c
	$(CC_COMPILE) $(@) $(<)

$(TARGETS) : % : %.$(OBJEXT)
	$(CC_BUILD_PROGRAM) $(@) $(^)

## ------------------------------------------------------------

#page
## ------------------------------------------------------------
## Autoconfiguration.
## ------------------------------------------------------------

.PHONY: nothing

# This target is to allow one to update the 'Makefile', if required
# without doing anything else; if the 'Makefile' does not need updating:
# nothing happens. This target is required by the 'develstuff.sh'
# script when querying the 'Makefile' for variable values. If the ':'
# command is not used, and the target is left with no commands, 'make'
# will print the message "Nothing to be done for 'nothing'"; we do not
# want to fill the user's terminal with this.
nothing:
	@:

Makefile: $(srcdir)/Makefile.in $(srcdir)/configure $(builddir)/config.status
	$(builddir)/config.status

config.status: $(srcdir)/configure
	$(builddir)/config.status --recheck

## ------------------------------------------------------------

#page
## ------------------------------------------------------------
## End.
## ------------------------------------------------------------

.PHONY: echo-variable echo-list-variable

echo-variable:
	@echo $($(VARIABLE))

echo-list-variable:
	@$(foreach f,$($(VARIABLE)),echo $(f);)

.NOEXPORT:


### end of file