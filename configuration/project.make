# @configure_input@
#

#page
## ------------------------------------------------------------
## Library.
## ------------------------------------------------------------

GSL_CFLAGS			= @GSL_CFLAGS@
GSL_LIBS			= @GSL_LIBS@

annealing_SRCDIR		= $(srcdir)/src/lib
annealing_PREREQUISITES		= internal.h annealing.h
annealing_CC_COMPILE_CPPFLAGS	= $(DEFS) $(CPPFLAGS) $(GSL_CFLAGS)
annealing_CC_SHLIB_LIBS		= $(LIBS) $(GSL_LIBS)
annealing_c_headers_PATTERNS	= annealing.h

$(eval $(call ds-c-library-extended,annealing))

#page
## ------------------------------------------------------------
## Tests.
## ------------------------------------------------------------

anntests_RULESET		= tests
anntests_SRCDIR			= $(srcdir)/tests
anntests_BUILDDIR		= $(builddir)/tests.d
anntests_PREREQUISITES		= annealing.h
anntests_CC_COMPILE_INCLUDES	= -I$(annealing_SRCDIR)

anntests_programs_RULESET		= tests
anntests_programs_BUILDDIR		= $(builddir)/tests.d
anntests_programs_CC_PROGRAM_LDFLAGS	= -L$(annealing_shlib_BUILDDIR)
anntests_programs_CC_PROGRAM_LIBS	= $(GSL_LIBS) -l$(annealing_LIBRARY_ID)

$(eval $(call ds-c-sources,anntests))
$(eval $(call ds-c-programs-no-install,anntests))

tests:
	$(foreach f,$(anntests_programs_PATHNAMES),\
	LD_LIBRARY_PATH=$(annealing_shlib_BUILDDIR) $(f);)

tests-mostlyclean:	anntests_programs-mostlyclean
tests-clean:		anntests_programs-clean

#page
## ------------------------------------------------------------
## Miscelleous stuff.
## ------------------------------------------------------------

compute_iterations_SRCDIR		= $(srcdir)/src/other
compute_iterations_RULESET		= nop
compute_iterations_BUILDDIR		= $(builddir)/other.d
compute_iterations_sinprog_RULESET	= nop
compute_iterations_sinprog_BUILDDIR	= $(compute_iterations_BUILDDIR)

$(eval $(call ds-c-sources,compute_iterations))
$(eval $(call ds-c-single-program-no-install,compute_iterations,compute-iterations))

.PHONY: compute-iterations

compute-iterations : compute_iterations_sinprog-all
	$(compute_iterations_sinprog_BUILDDIR)/compute-iterations

### end of file
# Local Variables:
# mode: makefile-gmake
# End:
