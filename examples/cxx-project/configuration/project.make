# @configure_input@
#

GSL_CFLAGS		= @GSL_CFLAGS@
GSL_LIBS		= @GSL_LIBS@

ANNEALING_INCLUDEDIR	= @ANNEALING_INCLUDEDIR@
ANNEALING_CPPFLAGS	= @ANNEALING_CPPFLAGS@
ANNEALING_LDFLAGS	= @ANNEALING_LDFLAGS@
ANNEALING_LIBS		= @ANNEALING_LIBS@

vpath %.h	$(ANNEALING_INCLUDEDIR)

progs_PREREQUISITES			= annealing.h internal.hpp
progs_CXX_COMPILE_INCLUDES		= $(ANNEALING_CPPFLAGS)
progs_sinprog_CXX_PROGRAM_LDFLAGS	= $(ANNEALING_LDFLAGS)
progs_sinprog_CXX_PROGRAM_LIBS		= $(GSL_LIBS) $(ANNEALING_LIBS)

$(progs_SRCDIR)/%.@OBJEXT@ : $(progs_SRCDIR)/%.hpp

$(eval $(call ds-cxx-sources,progs))
$(eval $(call ds-cxx-single-program,progs,sinc))

.PHONY: run

run: progs_sinprog-all
	$(progs_sinprog_PATHNAME)

### end of file
# Local Variables:
# mode: makefile-gmake
# End:
