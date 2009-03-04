# @configure_input@
#

GSL_CFLAGS		= @GSL_CFLAGS@
GSL_LIBS		= @GSL_LIBS@

ANNEALING_INCLUDEDIR	= @ANNEALING_INCLUDEDIR@
ANNEALING_CPPFLAGS	= @ANNEALING_CPPFLAGS@
ANNEALING_LDFLAGS	= @ANNEALING_LDFLAGS@
ANNEALING_LIBS		= @ANNEALING_LIBS@

vpath %.h	$(ANNEALING_INCLUDEDIR)

progs_PREREQUISITES			= annealing.h
progs_CC_COMPILE_INCLUDES		= $(ANNEALING_CPPFLAGS)
progs_programs_CC_PROGRAM_LDFLAGS	= $(ANNEALING_LDFLAGS)
progs_programs_CC_PROGRAM_LIBS		= $(GSL_LIBS) $(ANNEALING_LIBS)

$(eval $(call ds-c-sources,progs))
$(eval $(call ds-c-programs,progs))

.PHONY: run

run: progs_programs-all
	$(foreach f,$(progs_programs_PATHNAMES),$(f);)

### end of file
# Local Variables:
# mode: makefile-gmake
# End:
