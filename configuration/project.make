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

annealing_tests_PREREQUISITES			= annealing.h
annealing_tests_CC_COMPILE_INCLUDES		= -I$(annealing_SRCDIR)
annealing_tests_programs_CC_PROGRAM_LDFLAGS	= -L$(annealing_shlib_BUILDDIR)
annealing_tests_programs_CC_PROGRAM_LIBS	= $(GSL_LIBS) -l$(annealing_LIBRARY_ID)

$(eval $(call ds-c-test-programs,annealing))

tests:
	$(foreach f,$(annealing_tests_programs_PATHNAMES),\
	LD_LIBRARY_PATH=$(annealing_shlib_BUILDDIR) $(f);)

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
