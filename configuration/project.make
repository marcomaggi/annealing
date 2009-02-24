# Makefile.ds --

#page
## ------------------------------------------------------------
## Global variables.
## ------------------------------------------------------------

GSL_CFLAGS	= @GSL_CFLAGS@
GSL_LIBS	= @GSL_LIBS@

CPPFLAGS	+= $(GSL_CFLAGS)
LIBS		+= $(GSL_LIBS)

#page
## ------------------------------------------------------------
## Library.
## ------------------------------------------------------------

annealing_PREREQUISITES	= internal.h annealing.h

$(eval $(call ds-c-sources,annealing,$(srcdir)/src,*.c))
$(eval $(call ds-c-shared-library,annealing,$(annealing_LIBRARY_ID),$(annealing_TARGETS)))
$(eval $(call ds-c-static-library,annealing,$(annealing_LIBRARY_ID),$(annealing_TARGETS)))

## ------------------------------------------------------------

$(eval $(call ds-h-files-installer,annealing,$(srcdir)/src,annealing.h))

#page
## ------------------------------------------------------------
## Tests with shared library.
## ------------------------------------------------------------

shared_tests_BUILDDIR		= $(builddir)/shared-tests.d
shared_tests_CC_COMPILE		= $(CC) $(ALL_CFLAGS) -I$(annealing_SRCDIR) $(CFLAGS) $(CC_COMPILE_OUTPUT)
shared_tests_PREREQUISITES	= $(annealing_SRCDIR)/annealing.h
shared_tests_test_programs_BUILDDIR = $(shared_tests_BUILDDIR)

$(eval $(call ds-c-sources-nop,shared_tests,$(srcdir)/tests,*.c))
$(eval $(call ds-c-test-programs,shared_tests,$(shared_tests_TARGETS),\
	-l$(annealing_LIBRARY_ID) -L$(annealing_shared_library_BUILDDIR)))

## ------------------------------------------------------------

ifeq ($(strip $(ds_config_ENABLE_STATIC)),yes)

static_tests_BUILDDIR		= $(builddir)/static-tests.d
static_tests_CC_COMPILE		= $(CC) $(ALL_CFLAGS) -I$(annealing_SRCDIR) $(CFLAGS) $(CC_COMPILE_OUTPUT)
static_tests_PREREQUISITES	= $(annealing_SRCDIR)/annealing.h
static_tests_test_programs_BUILDDIR = $(static_tests_BUILDDIR)

$(eval $(call ds-c-sources-nop,static_tests,$(srcdir)/tests,*.c))
$(eval $(call ds-c-test-programs,static_tests,$(static_tests_TARGETS),$(annealing_static_library_LIBRARY)))

endif

## ------------------------------------------------------------

shared_test_NAMES	= $(basename $(notdir $(shared_tests_SOURCES)))

.PHONY: test-shared

test-shared: shared_tests_test_programs-all
	$(foreach f,$(shared_test_NAMES),\
	LD_LIBRARY_PATH=$(annealing_shared_library_BUILDDIR) $(shared_tests_test_programs_BUILDDIR)/$(f);)

test:		test-shared
test-clean:	shared_tests_test_programs-clean

## ------------------------------------------------------------

ifeq ($(strip $(ds_config_ENABLE_STATIC)),yes)

static_test_NAMES	= $(basename $(notdir $(static_tests_SOURCES)))

.PHONY: test-static

test-static: static_tests_test_programs-all
	$(foreach f,$(static_test_NAMES),\
	LD_LIBRARY_PATH=$(annealing_static_library_BUILDDIR) $(static_tests_test_programs_BUILDDIR)/$(f);)

test:		test-static
test-clean:	static_tests_test_programs-clean

endif

## ------------------------------------------------------------

#page
## ------------------------------------------------------------
## Miscelleous stuff.
## ------------------------------------------------------------

$(eval $(call ds-cc-build-program-no-libs,compute_iterations_program_CC_BUILD,\
	-L$(builddir)/library.d -l$(annealing_LIBRARY_ID)))

compute_iterations_BUILDDIR		= $(builddir)/misc.d
compute_iterations_program_BUILDDIR	= $(compute_iterations_BUILDDIR)
compute_iterations_PREREQUISITES	= $(annealing_SRCDIR)/annealing.h

$(eval $(call ds-c-sources-nop,compute_iterations,$(srcdir)/other,compute_outer_iterations.c))
$(eval $(call ds-c-program-no-install,compute_iterations,compute_iterations,$(compute_iterations_TARGETS)))

.PHONY: compute_iterations

compute_iterations : compute_iterations_program-all
	@LD_LIBRARY_PATH=$(annealing_shared_library_BUILDDIR) $(compute_iterations_program_BUILDDIR)/compute_iterations


### end of file
# Local Variables:
# mode: makefile
# End:
