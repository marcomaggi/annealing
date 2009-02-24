# @configure_input@
#
# Part of: DevelStuff
# Contents: library of functions for GNU Make
# Date: Mon Aug 20, 2007
#
# Abstract
#
#	This is  a library of functions  for GNU Make. It  will not work
#	with  other 'make' programs.   Along with  this file  you should
#	have  received  a documentation  file  in  Texinfo format  named
#	"infrastructure.texi".
#
# Copyright (c) 2007-2009 Marco Maggi <marcomaggi@gna.org>
#
# This program is  free software: you can redistribute  it and/or modify
# it under the  terms of the GNU General Public  License as published by
# the Free Software Foundation, either  version 3 of the License, or (at
# your option) any later version.
#
# This program  is distributed in the  hope that it will  be useful, but
# WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
# MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
# General Public License for more details.
#
# You  should have received  a copy  of the  GNU General  Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

#page
## --------------------------------------------------------------------
## Modules inclusion and configuration.
## --------------------------------------------------------------------

ds_include_BIN_RULES		= @DS_INCLUDE_BIN_RULES@
ds_include_DOC_RULES		= @DS_INCLUDE_DOC_RULES@
ds_include_DEV_RULES		= @DS_INCLUDE_DEV_RULES@

ds_include_AUTOCONF_DIRS	= @DS_INCLUDE_AUTOCONF_DIRS@
ds_include_DEVELSTUFF_DIRS	= @DS_INCLUDE_DEVELSTUFF_DIRS@

ds_config_VERSIONED_LAYOUT	?= @DS_CONFIG_VERSIONED_LAYOUT@

ds_config_ENABLE_DOC		?= @DS_CONFIG_ENABLE_DOC@
ds_config_ENABLE_DOC_INFO	?= @DS_CONFIG_ENABLE_DOC_INFO@
ds_config_ENABLE_DOC_HTML	?= @DS_CONFIG_ENABLE_DOC_HTML@
ds_config_ENABLE_DOC_DVI	?= @DS_CONFIG_ENABLE_DOC_DVI@
ds_config_ENABLE_DOC_PDF	?= @DS_CONFIG_ENABLE_DOC_PDF@
ds_config_ENABLE_DOC_PS		?= @DS_CONFIG_ENABLE_DOC_PS@

ds_config_ENABLE_STRIP		?= @DS_CONFIG_ENABLE_STRIP@

ds_config_ABI			?= @DS_CONFIG_ABI@
ds_config_USE_SUDO		?= @DS_CONFIG_ABI_SUDO@
ds_config_SLACKWARE_CHOWN	?= @DS_CONFIG_SLACKWARE_CHOWN@
ds_config_VERBOSE_MESSAGES	?= yes

# Compressor  to be  used when  creating a  tarball; it  is used  by the
# binary distribution rules.  Supported values:
#
#   bzip2		selects the Bzip2 compressor
#   gzip		selects the Gzip compressor
#
# if a different value is set, the compressor will default to 'gzip'.
#
ds_config_COMPRESSOR		?= @GZIP@

#page
## --------------------------------------------------------------------
## Package variables.
## --------------------------------------------------------------------

PACKAGE_NAME		= @PACKAGE_NAME@
PACKAGE_VERSION		= @PACKAGE_VERSION@
PKG_ID			?= $(PACKAGE_NAME)-$(PACKAGE_VERSION)
PKG_DIR			?= $(PACKAGE_NAME)/$(PACKAGE_VERSION)

#page
## ---------------------------------------------------------------------
## Main rules.
## ---------------------------------------------------------------------

define ds-included-phony-rules
$(1):	$$(addsuffix -$(1),$$(ds_INCLUDED_RULES))
endef

define ds-section-rules
.PHONY: $(1)							\
	$(1)-clean	$(1)-mostlyclean			\
	$(1)-install	$(1)-uninstall				\
	\
	$(1)-print-install-layout				\
	$(1)-print-install-dirs-layout				\
	$(1)-print-install-files-layout				\
	\
	$(1)-print-uninstall-script				\
	$(1)-print-uninstall-dirs-script			\
	$(1)-print-uninstall-files-script

$(1):
$(1)-clean:
$(1)-mostlyclean:
$(1)-install:
$(1)-uninstall:
$(1)-install-aux:
$(1)-uninstall-aux:

$(1)-print-install-layout:
$(1)-print-install-dirs-layout:
$(1)-print-install-files-layout:

$(1)-print-uninstall-script:
$(1)-print-uninstall-dirs-script:
$(1)-print-uninstall-files-script:

endef

ds_INCLUDED_RULES = \
	$(if $(filter yes,$(ds_include_BIN_RULES)),bin) \
	$(if $(filter yes,$(ds_include_DOC_RULES)),doc) \
	$(if $(filter yes,$(ds_include_DEV_RULES)),dev)

.PHONY:	all						\
	clean		mostlyclean			\
	install		uninstall			\
	\
	print-install-layout				\
	print-install-dirs-layout			\
	print-install-files-layout			\
	\
	print-uninstall-script				\
	print-uninstall-dirs-script			\
	print-uninstall-files-script

all:	$(ds_INCLUDED_RULES)
$(eval $(call ds-included-phony-rules,clean))
$(eval $(call ds-included-phony-rules,mostlyclean))
$(eval $(call ds-included-phony-rules,install))
$(eval $(call ds-included-phony-rules,uninstall))

$(eval $(call ds-included-phony-rules,print-install-layout))
$(eval $(call ds-included-phony-rules,print-install-dirs-layout))
$(eval $(call ds-included-phony-rules,print-install-files-layout))

$(eval $(call ds-included-phony-rules,print-uninstall-script))
$(eval $(call ds-included-phony-rules,print-uninstall-dirs-script))
$(eval $(call ds-included-phony-rules,print-uninstall-files-script))

$(eval $(call ds-section-rules,bin))
$(eval $(call ds-section-rules,doc))
$(eval $(call ds-section-rules,dev))

.PHONY: info html dvi pdf ps man

info html dvi pdf ps man:

.PHONY: abi abu bi bu

abi:	all bi
abu:	all bu

ifeq ($(ds_config_ABI),direct)
bi:	install
bu:	uninstall install
else ifeq ($(ds_config_ABI),bindist)
bi:	bindist bindist-install
bu:	uninstall bindist bindist-install
else ifeq ($(ds_config_ABI),slackware)
bi:	slackware slackware-install
bu:	slackware slackware-upgrade
else ifeq ($(ds_config_ABI),local-slackware)
bi:	local-slackware local-slackware-install
bu:	local-slackware local-slackware-upgrade
endif

.PHONY: clean-builddir

clean-builddir:
	-@printf '*** The build directory is: %s\n' "$(abspath $(builddir))";	\
	if test -d .git ;							\
	then echo '*** Refusing to clean directory with ".git" subdir';		\
	else read -p '*** Are you sure to clean it? (yes/no) ' ANSWER;		\
	     test "$${ANSWER}" = yes && $(RM) "$(builddir)"/*;			\
	     true;								\
	fi

.PHONY: nop nop-clean nop-mostlyclean

nop:
nop-clean:
nop-mostlyclean:

clean:		nop-clean
mostlyclean:	nop-mostlyclean

.PHONY: nothing

nothing:
# If  the ':'  command  is not  used, and  the  target is  left with  no
# commands,  "make" will  print  the  message "Nothing  to  be done  for
# 'nothing'"; we do not want to fill the user's terminal with this.
	@:

Makefile: $(srcdir)/Makefile.in $(srcdir)/configure $(builddir)/config.status
	$(builddir)/config.status

config.status: $(srcdir)/configure
	$(builddir)/config.status --recheck

.PHONY: echo-variable echo-list-variable

echo-variable:
	@echo $($(VARIABLE))

echo-list-variable:
	@$(foreach f,$($(VARIABLE)),echo $(f);)

# This  appears to  be ignored  by GNU  Make 3.81;  at least  it  is not
# mentioned in the Info documentation.   But it was used in the makefile
# of  Tcl  and it  seems  to  do no  harm.   On  the  Net the  following
# explanation was found:  "tell versions [3.59,3.63) of GNU  Make not to
# export all variables, otherwise a system limit (for SysV at least) may
# be exceeded".
.NOEXPORT:

#page
## ---------------------------------------------------------------------
## Directories.
## ---------------------------------------------------------------------

ds_meta_srcdir		?= $(srcdir)/meta
ds_meta_builddir	?= $(builddir)/meta.d
infrastructuredir	?= @INFRASTRUCTUREDIR@
configurationdir	?= @CONFIGURATIONDIR@

ifeq ($(strip $(TMPDIR)),)
TMPDIR		= /tmp
endif

ifeq ($(ds_include_AUTOCONF_DIRS),yes)
prefix		= @prefix@
exec_prefix	= @exec_prefix@
bindir		= @bindir@
datarootdir	= @datarootdir@
datadir		= @datadir@
docdir		= @datarootdir@/doc
includedir	= @includedir@
infodir		= @infodir@
htmldir		= @htmldir@
pdfdir		= @pdfdir@
psdir		= @psdir@
dvidir		= @dvidir@
libdir		= @libdir@
libexecdir	= @libexecdir@
localstatedir	= @localstatedir@
mandir		= @mandir@
sbindir		= @sbindir@
sharedstatedir	= @sharedstatedir@
sysconfdir	= @sysconfdir@
endif

ifeq ($(ds_include_DEVELSTUFF_DIRS),yes)
pkgdatadir	?= @pkgdatadir@
pkgdocdir	?= @pkgdocdir@
pkgexampledir	?= @pkgexampledir@
pkginfodir	?= @pkginfodir@
pkghtmldir	?= @pkghtmldir@
pkgpdfdir	?= @pkgpdfdir@
pkgpsdir	?= @pkgpsdir@
pkgdvidir	?= @pkgdvidir@
pkgincludedir	?= @pkgincludedir@
pkglibdir	?= @pkglibdir@
pkglibexecdir	?= @pkglibexecdir@
pkgsysconfdir	?= @pkgsysconfdir@
endif

#page
## ---------------------------------------------------------------------
## Miscellaneous.
## ---------------------------------------------------------------------

define ds-verbose
$(if $(ds_config_VERBOSE_MESSAGES),$(1),$(2))
endef

define ds-echo
@$(call ds-verbose,echo $(1))
endef

## ---------------------------------------------------------------------
## File system inspection.

define ds-drop-backup-files
$(filter-out %~,$(1))
endef

define ds-drop-equal-prefixed-files
$(foreach f,$(1),$(if $(filter =%,$(notdir $(f))),,$(f)))
endef

define ds-drop-unwanted-files
$(call ds-drop-equal-prefixed-files,$(call ds-drop-backup-files,$(1)))
endef

define ds-files-from-dir
$(call ds-drop-unwanted-files,$(wildcard $(addprefix $(1)/,$(if $(2),$(2),*))))
endef

define ds-replace-dir
$(addprefix $(1)/,$(notdir $(2)))
endef

define ds-glob
$(if $($(1)_SRCDIR),\
	$(call ds-files-from-dir,$($(1)_SRCDIR),$(2)),\
	$(error null source directory variable "$(1)_SRCDIR"))
endef

## ---------------------------------------------------------------------
## Source directory handling.

define ds-srcdir
$(1)_SRCDIR	?= $$(if $(2),$(2),$$(srcdir)/$(1))
# do not indenti this call
$$(call ds-assert-srcdir,$(1))
endef

define ds-assert-srcdir
$(if $($(1)_SRCDIR),\
	$(shell test -d $($(1)_SRCDIR) || \
		printf "*warning*: missing srcdir '%s'\n" $($(1)_SRCDIR) >&2),\
	$(error null source directory variable "$(1)_SRCDIR"))
endef

## ---------------------------------------------------------------------
## Build directory handling.

define ds-builddir
$(1)_BUILDDIR	?= $$(if $(2),$(2),$$(builddir)/$(1).d)

.PHONY: $(1)-make-builddir

$(1)-make-builddir:
# do not indenti this call
$$(call ds-make-builddir,$(1))
$(1)-all: $(1)-make-builddir
endef

define ds-make-builddir
$(if $($(1)_BUILDDIR),\
	$(shell test -d "$($(1)_BUILDDIR)" || $(MKDIR) "$($(1)_BUILDDIR)"),\
	$(error null build directory variable "$(1)_BUILDDIR"))
endef

## ---------------------------------------------------------------------
## Clean files

define ds-clean-files
$(if $($(1)_CLEANFILES),$(RM) $($(1)_CLEANFILES),\
	$(warning empty clean variable "$(1)_CLEANFILES"))
endef

define ds-mostlyclean-files
$(if $($(1)_MOSTLYCLEANFILES),$(RM) $($(1)_MOSTLYCLEANFILES),\
	$(warning empty mostly clean variable "$(1)_MOSTLYCLEANFILES"))
endef

define ds-default-clean-files-variables
$(1)_MOSTLYCLEANFILES	+= $$($(1)_TARGETS)
$(1)_CLEANFILES		+= $$($(1)_MOSTLYCLEANFILES)
endef

#page
## ---------------------------------------------------------------------
## Shortcuts for common module sets.
## ---------------------------------------------------------------------

define ds-full-head
$(eval $(call ds-common-programs))
$(eval $(call ds-tests))
$(eval $(call ds-generic-documentation))
endef

define ds-full-tail
$(eval $(call ds-texinfo-documentation))
$(eval $(call ds-uninstall-scripts))
$(eval $(call ds-meta-scripts))
$(eval $(call ds-pkg-config))
$(eval $(call ds-autoconf))
$(eval $(call ds-config-inspection-script))
$(eval $(call ds-source-distribution))
$(eval $(call ds-binary-distribution))
$(eval $(call ds-slackware-distribution))
endef

#page
## ---------------------------------------------------------------------
## Programs.
## ---------------------------------------------------------------------

define ds-common-programs
BASH_PROGRAM	= @BASH_PROGRAM@
SHELL		= @SHELL@
@SET_MAKE@
MAKE_SILENT	= $$(MAKE) --silent
MAKE_NODIR	= $$(MAKE) --no-print-directory
BZIP		= @BZIP@
CAT		= @CAT@
CP		= @CP@ --force --verbose --preserve=mode,timestamp --
DATE		= @DATE@
GREP		= @GREP@
GAWK		= @GAWK@
GZIP		= @GZIP@
M4		= @M4@
MAKEINFO	= @MAKEINFO@
MKDIR		= @MKDIR@ --parents --verbose
MV		= @MV@ --verbose --
RM		= @RM@ --force --recursive --verbose --
RM_FILE		= @RM@ --force --verbose --
RM_SILENT	= @RM@ --force --recursive --
RMDIR		= @RMDIR@ --parents --ignore-fail-on-non-empty --
SED		= @SED@
SYMLINK		= @SYMLINK@ --symbolic
TAR		= @TAR@
TEXI2PDF	= @TEXI2PDF@
DVIPS		= @DVIPS@
SUDO		= @SUDO@

INSTALL			= @INSTALL@
INSTALL_DIR_MODE	?= 0755
INSTALL_BIN_MODE	?= 0555
INSTALL_DATA_MODE	?= 0444
INSTALL_LIB_MODE	?= 0444

INSTALL_DIR	= $$(INSTALL) -p -m $$(INSTALL_DIR_MODE) -d
INSTALL_BIN	= $$(INSTALL) -p -m $$(INSTALL_BIN_MODE)
INSTALL_DATA	= $$(INSTALL) -p -m $$(INSTALL_DATA_MODE)
endef

#page
## ---------------------------------------------------------------------
## Source modules.
## ---------------------------------------------------------------------

define ds-module
# $(1) - is the module identifier
# $(2) - is the main section: bin, dev, doc, nop, test, or whatever
# $(3) - is the installation mode, second argument to 'ds-install-module'

.PHONY: $$(addprefix $(1)-,	all clean mostlyclean				\
				install preinstall install-body postinstall	\
				uninstall uninstall-aux				\
				\
				print-install-files-layout			\
				print-install-dirs-layout			\
				print-install-layout				\
				\
				print-install-files-layout-aux			\
				print-install-dirs-layout-aux			\
				print-install-layout-aux			\
				\
				print-uninstall-files-script			\
				print-uninstall-dirs-script			\
				print-uninstall-script				\
				\
				print-uninstall-files-script-aux		\
				print-uninstall-dirs-script-aux			\
				print-uninstall-script-aux)

$(1)-all: $$($(1)_TARGETS)
$(1)-mostlyclean:			; -@$$(call ds-mostlyclean-files,$(1))
$(1)-clean:				; -@$$(call ds-clean-files,$(1))
$(1)-install:	$(1)-preinstall $(1)-install-body $(1)-postinstall
$(1)-uninstall:	$(1)-uninstall-aux	; @$$(call ds-uninstall-module,$(1))
$(1)-install-aux:
$(1)-uninstall-aux:

$(1)-preinstall:
$(1)-install-body: ; @$$(call ds-install-module,$(1),$(if $(3),$(3),DATA))
$(1)-postinstall:

$(1)-print-install-files-layout:	$(1)-print-install-files-layout-aux
	@$$(call ds-module-print-files-layout,$(1))
$(1)-print-install-dirs-layout:		$(1)-print-install-dirs-layout-aux
	@$$(call ds-module-print-dirs-layout,$(1))
$(1)-print-install-layout:		$(1)-print-install-layout-aux
	@$$(call ds-module-print-layout,$(1))

$(1)-print-install-files-layout-aux:
$(1)-print-install-dirs-layout-aux:
$(1)-print-install-layout-aux:		$(1)-print-install-files-layout-aux \
					$(1)-print-install-dirs-layout-aux

$(1)-print-uninstall-files-script:	$(1)-print-uninstall-files-script-aux
	@$$(call ds-module-print-uninstall-files-script,$(1))
$(1)-print-uninstall-dirs-script:	$(1)-print-uninstall-dirs-script-aux
	@$$(call ds-module-print-uninstall-dirs-script,$(1))
$(1)-print-uninstall-script:		$(1)-print-uninstall-script-aux
	@$$(call ds-module-print-uninstall-script,$(1))

$(1)-print-uninstall-files-script-aux:
$(1)-print-uninstall-dirs-script-aux:
$(1)-print-uninstall-script-aux:	$(1)-print-uninstall-files-script-aux \
					$(1)-print-uninstall-dirs-script-aux

# ------------------------------------------------------------

$(2):			$(1)-all
$(2)-mostlyclean:	$(1)-mostlyclean
$(2)-clean:		$(1)-clean
$(2)-install:		$(1)-install
$(2)-uninstall:		$(1)-uninstall

$(2)-print-install-files-layout:	$(1)-print-install-files-layout
$(2)-print-install-dirs-layout:		$(1)-print-install-dirs-layout
$(2)-print-install-layout:		$(1)-print-install-layout

$(2)-print-uninstall-files-script:	$(1)-print-uninstall-files-script
$(2)-print-uninstall-dirs-script: 	$(1)-print-uninstall-dirs-script
$(2)-print-uninstall-script:		$(1)-print-uninstall-script

endef

define ds-module-no-install
# $(1) - is the module identifier
# $(2) - is the main section: bin, dev, doc, nop, test, or whatever
.PHONY: $$(addprefix $(1)-,all mostlyclean clean)

$(1)-all: $$($(1)_TARGETS)
$(1)-mostlyclean:	; -@$$(call ds-mostlyclean-files,$(1))
$(1)-clean:		; -@$$(call ds-clean-files,$(1))

$(2):			$(1)-all
$(2)-mostlyclean:	$(1)-mostlyclean
$(2)-clean:		$(1)-clean
endef

#page
## ---------------------------------------------------------------------
## Canned installation commands.
## ---------------------------------------------------------------------

define ds-permissions
$(1)_OWNER	= $(2)
$(1)_GROUP	= $(3)
$(1)_FMODE	= $(4)
$(1)_DMODE	= $(5)
endef

## ---------------------------------------------------------------------
## Module's directory and files installation.

define ds-install-directory
$(if $($(1)_INSTDIR),$(INSTALL) \
	-m $(if $($(1)_DMODE),$($(1)_DMODE),$(INSTALL_DIR_MODE)) \
        $(if $($(1)_OWNER),-o $($(1)_OWNER)) \
        $(if $($(1)_GROUP),-g $($(1)_GROUP)) \
	-d $(DESTDIR)$($(1)_INSTDIR),\
	$(error null install directory variable "$(1)_INSTDIR"))
endef

define ds-install-files
$(if $($(1)_INSTLST),\
	$(if $($(1)_INSTDIR),\
		$(INSTALL) \
			-m $(if $($(1)_FMODE),$($(1)_FMODE),$(INSTALL_$(2)_MODE)) \
			$(if $($(1)_OWNER),-o $($(1)_OWNER)) \
			$(if $($(1)_GROUP),-g $($(1)_GROUP)) \
			$($(1)_INSTLST) $(DESTDIR)$($(1)_INSTDIR),\
		$(error null install directory variable "$(1)_INSTDIR")),\
	$(error empty install list variable "$(1)_INSTLST"))
endef

## ---------------------------------------------------------------------
## Module's installation functions.

define ds-install-module
$(call ds-echo,'## ---------------------------------------------------------------------')
$(call ds-echo,'## Installing $(1) files...')
$(call ds-install-directory,$(1))
$(call ds-install-files,$(1),$(2))
$(call ds-echo,'## done.')
endef

define ds-install-data
$(call ds-install-module,$(1),DATA)
endef

define ds-install-bin
$(call ds-install-module,$(1),BIN)
endef

define ds-install-lib
$(call ds-install-module,$(1),LIB)
endef

## ---------------------------------------------------------------------
## Module's uninstall functions.

define ds-uninstall-files
$(foreach f,$($(1)_INSTLST),$(RM_FILE) $(DESTDIR)$($(1)_INSTDIR)/$(notdir $(f));)
endef

define ds-uninstall-dirs
$(RMDIR) $(DESTDIR)$($(1)_INSTDIR)
endef

define ds-uninstall-module
$(call ds-uninstall-files,$(1))
$(call ds-uninstall-dirs,$(1))
endef

## ---------------------------------------------------------------------
## Module's installation layout inspection.

define ds-module-print-files-layout
$(foreach f,$($(1)_INSTLST),echo $($(1)_INSTDIR)/$(notdir $(f));)
endef

define ds-module-print-dirs-layout
echo $($(1)_INSTDIR)
endef

define ds-module-print-layout
$(call ds-module-print-files-layout,$(1))
$(call ds-module-print-dirs-layout,$(1))
endef

## ---------------------------------------------------------------------
## Module's uninstall scripts.

define ds-module-print-uninstall-files-script
$(foreach f,$($(1)_INSTLST),echo $(RM_FILE) $($(1)_INSTDIR)/$(notdir $(f));)
endef

define ds-module-print-uninstall-dirs-script
echo $(RMDIR) $($(1)_INSTDIR)
endef

define ds-module-print-uninstall-script
$(call ds-module-print-uninstall-files-script,$(1))
$(call ds-module-print-uninstall-dirs-script,$(1))
endef

#page
define ds-generic-documentation
$$(eval $$(call ds-srcdir,ds_doc_generic,$$(srcdir)))
$$(eval $$(call ds-builddir,ds_doc_generic,$$(builddir)/doc-generic.d))

ds_doc_generic_PATTERNS	?= README* COPYING license.terms INSTALL* BUGS \
			   NEWS ChangeLog DESCRIPTION.txt TODO
ds_doc_generic_SOURCES	= $$(call ds-glob,ds_doc_generic,$$(ds_doc_generic_PATTERNS))
ds_doc_generic_TARGETS	= $$(call ds-replace-dir,$$(ds_doc_generic_BUILDDIR),\
				$$(ds_doc_generic_SOURCES:=.gz))
ds_doc_generic_INSTLST	= $$(ds_doc_generic_TARGETS)
ds_doc_generic_INSTDIR	= $$(pkgdocdir)

ds_doc_generic_MOSTLYCLEANFILES	= $$(ds_doc_generic_TARGETS)
ds_doc_generic_CLEANFILES	= $$(ds_doc_generic_MOSTLYCLEANFILES)

$$(eval $$(call ds-module,ds_doc_generic,doc,DATA))

$$(ds_doc_generic_TARGETS): $$(ds_doc_generic_BUILDDIR)/%.gz : $$(ds_doc_generic_SRCDIR)/%
	$$(GZIP) --best --stdout $$(<) >$$(@)

endef

#page
define ds-texinfo-documentation
ifeq ($$(ds_config_ENABLE_DOC),yes)

$$(eval $$(call ds-srcdir,ds_texi,$$(srcdir)/doc))
$$(eval $$(call ds-builddir,ds_texi,$$(builddir)/texinfo.d))

vpath	%.texi		$$(ds_texi_SRCDIR)
vpath	%.texiinc	$$(ds_texi_SRCDIR)

ds_texi_SOURCES		= $$(call ds-glob,ds_texi,*.texi)

DS_TEXI_FLAGS		= -I $$(ds_texi_SRCDIR)		\
			  -I $$(ds_texi_BUILDDIR)	\
			  -I $$(infrastructuredir)
DS_TEXI2INFO_FLAGS	= $$(DS_TEXI_FLAGS) --no-split
DS_TEXI2HTML_FLAGS	= $$(DS_TEXI_FLAGS) --no-split --html
DS_TEXI2DVI_FLAGS	= $$(DS_TEXI_FLAGS) --dvi --tidy \
				--build-dir=$$(ds_texi_BUILDDIR)
DS_TEXI2PDF_FLAGS	= $$(DS_TEXI_FLAGS) --dvipdf --tidy \
				--build-dir=$$(ds_texi_BUILDDIR)

ds_texi_PREREQ		= $$(ds_texi_BUILDDIR)/version.texiinc \
				  $$(wildcard $$(ds_texi_SRCDIR)/*.texiinc) \
				  $$(ds_texi_AUX_PREREQ)
ds_texi_CLEANFILES	= $$(ds_texi_BUILDDIR)/version.texiinc

.PHONY: ds-texinfo-builddir

ds-texinfo-builddir:
	test -d $$(ds_texi_BUILDDIR) || $$(MKDIR) $$(ds_texi_BUILDDIR)

$$(ds_texi_BUILDDIR)/version.texiinc: Makefile
# Placing here the command for the  build dir prevents the targets to be
# rebuild every time the "doc" rule is invoked.  This is because writing
# a  file in  the build  directory  changes the  directory access  time.
# Neither  a phony rule  nor an  ordinary rule  would solve  the problem
# (trust me, I tried them).
	@$(MAKE) ds-texinfo-builddir
	echo -e "@macro version{}\n$$(PACKAGE_VERSION)\n@end macro\n" >$$(@)

$$(ds_texi_BUILDDIR)/%.info: $$(ds_texi_SRCDIR)/%.texi $$(ds_texi_PREREQ)
	$$(MAKEINFO) $$(DS_TEXI2INFO_FLAGS) -o $$(@) $$(<)

$$(ds_texi_BUILDDIR)/%.info.gz: $$(ds_texi_BUILDDIR)/%.info
	$$(GZIP) --force --best $$(<)

$$(ds_texi_BUILDDIR)/%.html: $$(ds_texi_SRCDIR)/%.texi $$(ds_texi_PREREQ)
	$$(MAKEINFO) $$(DS_TEXI2HTML_FLAGS) -o $$(@) $$(<)

$$(ds_texi_BUILDDIR)/%.dvi: $$(ds_texi_SRCDIR)/%.texi $$(ds_texi_PREREQ)
	$$(TEXI2DVI) $$(DS_TEXI2DVI_FLAGS) -o $$(@) $$(<)

$$(ds_texi_BUILDDIR)/%.pdf: $$(ds_texi_SRCDIR)/%.texi $$(ds_texi_PREREQ)
	$$(TEXI2PDF) $$(DS_TEXI2PDF_FLAGS) -o $$(@) $$(<)

$$(ds_texi_BUILDDIR)/%.ps: $$(ds_texi_BUILDDIR)/%.dvi
	cd $$(ds_texi_BUILDDIR) && $$(DVIPS) $$(notdir $$(<)) -o

$$(ds_texi_BUILDDIR)/%.ps.gz: $$(ds_texi_BUILDDIR)/%.ps
	$$(GZIP) --force --best $$(<)

## ---------------------------------------------------------------------

ifeq ($$(ds_config_ENABLE_DOC_INFO),yes)

ds_texi_INFO_TARGETS	= $$(call ds-replace-dir,$$(ds_texi_BUILDDIR),\
				$$(ds_texi_SOURCES:.texi=.info.gz))
ds_texi_INFO_INSTLST	= $$(ds_texi_INFO_TARGETS)
ds_texi_INFO_INSTDIR	= \
	$$(if $$(filter yes,$$(ds_config_VERSIONED_LAYOUT)),$$(pkginfodir),$$(infodir))

ds_texi_INFO_MOSTLYCLEANFILES	= $$(ds_texi_INFO_TARGETS)
ds_texi_INFO_CLEANFILES		= $$(ds_texi_INFO_MOSTLYCLEANFILES) \
				  $$(ds_texi_CLEANFILES)

$$(eval $$(call ds-module,ds_texi_INFO,doc,DATA))

info: ds_texi_INFO-all

endif # ds_config_ENABLE_DOC_INFO = yes

## ---------------------------------------------------------------------

ifeq ($$(ds_config_ENABLE_DOC_HTML),yes)

ds_texi_HTML_TARGETS	= $$(call ds-replace-dir,$$(ds_texi_BUILDDIR),\
				$$(ds_texi_SOURCES:.texi=.html))
ds_texi_HTML_INSTLST	= $$(ds_texi_HTML_TARGETS)
ds_texi_HTML_INSTDIR	= \
	$$(if $$(filter yes,$$(ds_config_VERSIONED_LAYOUT)),$$(pkginfodir),$$(infodir))

ds_texi_HTML_MOSTLYCLEANFILES	= $$(ds_texi_HTML_TARGETS)
ds_texi_HTML_CLEANFILES		= $$(ds_texi_HTML_MOSTLYCLEANFILES) \
				  $$(ds_texi_CLEANFILES)

$$(eval $$(call ds-module,ds_texi_HTML,doc,DATA))

html: ds_texi_HTML-all

endif # ds_config_ENABLE_DOC_HTML = yes

## ---------------------------------------------------------------------

ifeq ($$(ds_config_ENABLE_DOC_DVI),yes)

ds_texi_DVI_TARGETS	= $$(call ds-replace-dir,$$(ds_texi_BUILDDIR),\
				$$(ds_texi_SOURCES:.texi=.dvi))
ds_texi_DVI_INSTLST	= $$(ds_texi_DVI_TARGETS)
ds_texi_DVI_INSTDIR	= \
	$$(if $$(filter yes,$$(ds_config_VERSIONED_LAYOUT)),$$(pkginfodir),$$(infodir))

ds_texi_DVI_MOSTLYCLEANFILES	= $$(ds_texi_DVI_TARGETS)
ds_texi_DVI_CLEANFILES		= $$(ds_texi_DVI_MOSTLYCLEANFILES) \
				  $$(ds_texi_BUILDDIR)/*.t2d \
				  $$(ds_texi_CLEANFILES)

$$(eval $$(call ds-module,ds_texi_DVI,doc,DATA))

dvi: ds_texi_DVI-all

endif # ds_config_ENABLE_DOC_DVI = yes

## ---------------------------------------------------------------------

ifeq ($$(ds_config_ENABLE_DOC_PDF),yes)

ds_texi_PDF_TARGETS	= $$(call ds-replace-dir,$$(ds_texi_BUILDDIR),\
				$$(ds_texi_SOURCES:.texi=.pdf))
ds_texi_PDF_INSTLST	= $$(ds_texi_PDF_TARGETS)
ds_texi_PDF_INSTDIR	= \
	$$(if $$(filter yes,$$(ds_config_VERSIONED_LAYOUT)),$$(pkginfodir),$$(infodir))

ds_texi_PDF_MOSTLYCLEANFILES	= $$(ds_texi_PDF_TARGETS)
ds_texi_PDF_CLEANFILES		= $$(ds_texi_PDF_MOSTLYCLEANFILES) \
				  $$(ds_texi_BUILDDIR)/*.t2d \
				  $$(ds_texi_CLEANFILES)

$$(eval $$(call ds-module,ds_texi_PDF,doc,DATA))

pdf: ds_texi_PDF-all

endif # ds_config_ENABLE_DOC_PDF = yes

## ---------------------------------------------------------------------

ifeq ($$(ds_config_ENABLE_DOC_PS),yes)

ds_texi_PS_TARGETS	= $$(call ds-replace-dir,$$(ds_texi_BUILDDIR),\
				$$(ds_texi_SOURCES:.texi=.ps.gz))
ds_texi_PS_INSTLST	= $$(ds_texi_PS_TARGETS)
ds_texi_PS_INSTDIR	= \
	$$(if $$(filter yes,$$(ds_config_VERSIONED_LAYOUT)),$$(pkginfodir),$$(infodir))

ds_texi_PS_MOSTLYCLEANFILES	= $$(ds_texi_PS_TARGETS)
ds_texi_PS_CLEANFILES		= $$(ds_texi_PS_MOSTLYCLEANFILES) \
				  $$(ds_texi_BUILDDIR)/*.t2d \
				  $$(ds_texi_CLEANFILES)

$$(eval $$(call ds-module,ds_texi_PS,doc,DATA))

ps: ds_texi_PS-all

endif # ds_config_ENABLE_DOC_PDF = yes

## ---------------------------------------------------------------------

endif # ds_config_ENABLE_DOC = yes
endef

#page
## ---------------------------------------------------------------------
## Predefined modules: meta scripts.
## ---------------------------------------------------------------------

# Synopsis:
#
#	$(eval $(call ds-meta-scripts))
#
# Description:
#
#  Add a module to install package handling scripts. Meta scripts are:
#
#	preinstall		to be executed before installing the package
#				(for example to add users and groups)
#
#	postinstall		to be executed after package installation
#				(for example to configure the pakcage)
#
#	preremoval		to be executed before removing the package
#				(for example to remove programs from menu
#				infrastructures)
#
#	postremoval		to be executed after removing the package
#				(for example to remove users and groups)
#
#  It is fine if only some (or none) of the scripts exist in the source tree.

define ds-meta-scripts
$$(eval $$(call ds-srcdir,ds_meta_scripts,$$(ds_meta_srcdir)))
$$(eval $$(call ds-builddir,ds_meta_scripts,$$(ds_meta_builddir)))

ds_meta_scripts_NAMES	= preinstall postinstall preremoval postremoval
ds_meta_scripts_SOURCES = $$(call ds-glob,ds_meta_scripts,$$(addsuffix .in,$$(ds_meta_scripts_NAMES)))
ds_meta_scripts_INSTLST	= $$(call ds-replace-dir,$$(ds_meta_scripts_BUILDDIR),$$(ds_meta_scripts_SOURCES:.in=))
ds_meta_scripts_INSTDIR	= $$(pkglibexecdir)

ifneq ($$(strip $$(ds_meta_scripts_SOURCES)),)
$$(eval $$(call ds-module,ds_meta_scripts,bin,BIN))
endif

endef

## ---------------------------------------------------------------------

#page
## ---------------------------------------------------------------------
## Predefined modules: pkg-config meta data files.
## ---------------------------------------------------------------------

# Synopsis:
#
#	$(eval $(call ds-pkg-config,<NAMES>))
#
# Description:
#
#  Add a module to install the meta data files for pkg-config.
#  More than one meta file can be selected by setting the
#  <NAMES> parameter to the list of file names with the ".pc"
#  extension stripped.
#
#  Each of the file names will be "$(builddir)/meta.d/<NAME>.pc",
#  and it should be produced by "configure" by putting the line
#
#	AC_CONFIG_FILES(meta.d/<NAME>.pc:meta/<NAME>.pc.in)
#
#  in the "configure.ac" file. It is fine if the files do not
#  not exist. If <NAMES> is not used: by default an attempt
#  is done to use "$(PACKAGE_NAME).pc".

define ds-pkg-config
$$(eval $$(call ds-srcdir,ds_pkg_config,$$(ds_meta_srcdir)))
$$(eval $$(call ds-builddir,ds_pkg_config,$$(ds_meta_builddir)))

ds_pkg_config_NAMES	= $$(addsuffix .pc,$$(if $(2),$(2),$$(PACKAGE_NAME)))
ds_pkg_config_SOURCES	= $$(call ds-glob,ds_pkg_config,$$(addsuffix .in,$$(ds_pkg_config_NAMES)))
ds_pkg_config_INSTLST	= $$(call ds-replace-dir,$$(ds_pkg_config_BUILDDIR),$$(ds_pkg_config_SOURCES:.in=))
ds_pkg_config_INSTDIR	= $$(libdir)/pkgconfig

ifneq ($$(strip $$(ds_pkg_config_SOURCES)),)
$$(eval $$(call ds-module,ds_pkg_config,bin,DATA))
endif

endef

## ---------------------------------------------------------------------

#page
## ---------------------------------------------------------------------
## Predefined modules: GNU Autoconf macro files.
## ---------------------------------------------------------------------

# Synopsis:
#
#	$(eval $(call ds-autoconf,<NAMES>))
#
# Description:
#
#  Add a module to install macro files GNU Autoconf. These
#  files are meant to be included in foreign packages that
#  depend on this pakcage.
#
#  More than one macro file can be selected by setting the
#  <NAMES> parameter to the list of file names with the ".m4"
#  extension stripped.
#
#  The files are searched  in the directory "$(srcdir)/autoconf".  It is
#  fine  if the  files do  not not  exist. If  <NAMES> is  not  used: by
#  default an attempt is done to install "$(PACKAGE_NAME).m4".

define ds-autoconf
$$(eval $$(call ds-srcdir,ds_autoconf,$$(ds_meta_srcdir)/autoconf))

ds_autoconf_NAMES	= $$(addsuffix .m4,$$(if $(2),$(2),$$(PACKAGE_NAME)))
ds_autoconf_INSTLST	= $$(call ds-glob,ds_autoconf,$$(ds_autoconf_NAMES))
ds_autoconf_INSTDIR	= $$(datadir)/aclocal/$$(PKG_DIR)

ifneq ($$(strip $$(ds_autoconf_INSTLST)),)
$$(eval $$(call ds-module,ds_autoconf,dev,DATA))
endif

endef

#page
## ---------------------------------------------------------------------
## Predefined modules: uninstall scripts.
## ---------------------------------------------------------------------

# *** WARNING ***
#
#  When using this function: it must be the last thing of the Makefile.

define ds-uninstall-scripts
ifeq ($$(ds_include_BIN_RULES),yes)
$$(eval $$(call ds-private-uninstall-scripts,bin))
endif

ifeq ($$(ds_include_DOC_RULES),yes)
$$(eval $$(call ds-private-uninstall-scripts,doc,doc-))
endif

ifeq ($$(ds_include_DEV_RULES),yes)
$$(eval $$(call ds-private-uninstall-scripts,dev,dev-))
endif
endef

## ---------------------------------------------------------------------

# 1 - the section identifier
# 2 - the package name section
define ds-private-uninstall-scripts

$$(eval $$(call ds-builddir,ds_uninstall_$(1),$$(builddir)/uninstall.d))

ds_uninstall_$(1)_PACKAGE	= $$(PACKAGE_NAME)-$(2)$$(PACKAGE_VERSION)
ds_uninstall_$(1)_NAME		= uninstall-$$(ds_uninstall_$(1)_PACKAGE).sh
ds_uninstall_$(1)_PATHNAME	= $$(ds_uninstall_$(1)_BUILDDIR)/$$(ds_uninstall_$(1)_NAME)

ds_uninstall_$(1)_TARGETS	= $$(ds_uninstall_$(1)_PATHNAME)
ds_uninstall_$(1)_INSTLST	= $$(ds_uninstall_$(1)_TARGETS)
ds_uninstall_$(1)_INSTDIR	= $$(pkglibexecdir)

ds_uninstall_$(1)_MOSTLYCLEANFILES	= $$(ds_uninstall_$(1)_TARGETS)
ds_uninstall_$(1)_CLEANFILES		= $$(ds_uninstall_$(1)_MOSTLYCLEANFILES)

$$(eval $$(call ds-module,ds_uninstall_$(1),$(1),BIN))

$$(ds_uninstall_$(1)_PATHNAME):
	$$(call ds-echo,'## ---------------------------------------------------------------------')
	$$(call ds-echo,'## Building $(1) uninstall script...')
# This is not required because the first 'echo' outputs with '>'.
#	-@test -f $$(@) && $(RM) $$(@)
	@echo '#!/bin/sh'							>$$(@)
	@echo '#'								>>$$(@)
	@echo '# Executing this script will remove the package: $$(ds_uninstall_$(1)_PACKAGE).'	>>$$(@)
	@echo '#'								>>$$(@)
	@echo '# *** WARNING ***'						>>$$(@)
	@echo '#'								>>$$(@)
	@echo '# Do not run this script if you use a package management system like'	>>$$(@)
	@echo '# the one of Slackware Linux.  Rely on that for package removal.'	>>$$(@)
	@echo									>>$$(@)
	@$(MAKE) --silent $(1)-print-uninstall-script >>$$(@)
	@printf '\n### end of file\n' >>$$(@)
	$$(call ds-echo,'## done.')

endef

#page
## ---------------------------------------------------------------------
## Predefined modules: package config inspection script.
## ---------------------------------------------------------------------

# Synopsis:
#
#	$(eval $(call ds-config-inspection-sript))
#
# Description:
#
#  Install the configuration inspection script: a program that
#  outputs the installation directory, include files directory,
#  package version, etc.

define ds-config-inspection-script
$$(eval $$(call ds-srcdir,ds_config_script,$$(ds_meta_srcdir)))
$$(eval $$(call ds-builddir,ds_config_script,$$(ds_meta_builddir)))

ds_config_script_NAME	 = $$(PACKAGE_NAME)-config
ds_config_script_INSTLST = $$(ds_config_script_BUILDDIR)/$$(ds_config_script_NAME)
ds_config_script_INSTDIR = $$(bindir)

$$(eval $$(call ds-module,ds_config_script,bin,BIN))
endef

## ---------------------------------------------------------------------

#page
## ---------------------------------------------------------------------
## Predefined modules: tests.
## ---------------------------------------------------------------------

define ds-tests
.PHONY: test  test-clean  test-mostlyclean
.PHONY: tests tests-clean tests-mostlyclean
.PHONY: check

test:
tests:			test
check:			test

test-clean:
tests-clean:		test-clean
clean:			test-clean

test-mostlyclean:	test-clean
tests-mostlyclean:	test-mostlyclean
mostlyclean:		test-mostlyclean
endef

# Synopsis:
#
#	$(eval $(call ds-tcl-programs))
#	$(eval $(call ds-tests))
#	$(eval $(call ds-tcl-tests))
#
# Description:
#
#  Add rules for  testing Tcl scripts and packages,  using the 'tcltest'
#  package.  From  the command line  of 'make' the  variable 'TESTFLAGS'
#  can be used to configure the 'tcltest' package.

define ds-tcl-tests
ds_tcl_TESTDIR			?= $$(srcdir)/tests
ds_tcl_TESTMAIN			?= $$(ds_tcl_TESTDIR)/all.tcl
ds_tcl_test_TARGETS		?=
ds_tcl_test_MOSTLYCLEANFILES	?=
ds_tcl_test_CLEANFILES		?=

ds_tcl_test_ENV		= 			\
	TMPDIR=$$(TMPDIR)			\
	srcdir=$$(abspath $$(srcdir))		\
	builddir=$$(abspath $$(builddir))

.PHONY: tcltest tcltests

tcltest tcltests: $$(ds_tcl_test_TARGETS)
	test -f $$(ds_tcl_TESTMAIN) && \
	$$(ds_tcl_test_ENV) $$(TCLSH) $$(ds_tcl_TESTMAIN) $$(TESTFLAGS)

tcltest-clean:
	-$$(RM) $$(ds_tcl_test_CLEANFILES)
tcltest-mostlyclean:
	-$$(RM) $$(ds_tcl_test_MOSTLYCLEANFILES)

test:			tcltest
test-clean:		tcltest-clean
test-mostlyclean:	tcltest-mostlyclean
endef

#page
## ---------------------------------------------------------------------
## Predefined modules: examples.
## ---------------------------------------------------------------------

# Synopsis:
#
#	$(eval $(call ds-examples))
#
# Description:
#
#  Add rules to drive examples building and installation.

define ds-examples
ds_examples_TARGETS	?=

ds_examples_INSTLST	?=
ds_examples_INSTDIR	= $$(pkgexampledir)

$$(eval $$(call ds-default-clean-files-variables,ds_examples))
$$(eval $$(call ds-module,ds_examples,doc,DATA))

.PHONY: example  example-clean  example-mostlyclean
.PHONY: examples examples-clean examples-mostlyclean

examples:		ds_examples-all
examples-clean:		ds_examples-clean
examples-mostlyclean:	ds_examples-mostlyclean

example:		examples
example-clean:		examples-clean
example-mostlyclean:	examples-mostlyclean
endef

#page

define ds-c-language

ds_config_ENABLE_STATIC	?= @ds_config_ENABLE_STATIC@
ds_config_ENABLE_SHARED	?= @ds_config_ENABLE_SHARED@
ds_config_ENABLE_STRIP	?= yes

CC			= @CC@
CPP			= @CPP@
AR			= @AR@ rc
RANLIB			= @RANLIB@
STRIP			= @STRIP@
GDB			= @GDB@

DEFS			?= @DEFS@
INCLUDES		= -I. -I$$(srcdir)
CPPFLAGS		?= @CPPFLAGS@ $$(DEFS) $$(INCLUDES)

# Notice  that '-std=c99'  is appended  to  'CC' by  the Autoconf  macro
# 'AC_PROG_CC_C99'.
C_DEFAULT		?= -pipe
C_WARNINGS		?= -Wall -W -Wextra -pedantic			\
			   -Wmissing-prototypes				\
			   -Wpointer-arith -Wcast-qual -Wcast-align	\
			   -Wwrite-strings -Wnested-externs		\
			   -Wstrict-prototypes -Wshadow -fno-common

CCFLAGS			?= $$(C_DEFAULT) $$(C_WARNINGS)
CFLAGS			?= @CFLAGS@
LIBS			?= @LIBS@
LDFLAGS			?= @LDFLAGS@
LDFLAGS_RPATH		?= @LDFLAGS_RPATH@
LDFLAGS_DL		?= @LDFLAGS_DL@
OBJEXT			= @OBJEXT@
CC_COMPILE_OUTPUT	?= $$(if @NO_MINUS_C_MINUS_O@,-o,-c -o)
CC_BUILD_OUTPUT		?= -o

endef

#page

# $(1) - the identifier of the module
# $(2) - a list of linker options
define ds-cc-compile
$(1)_CC_COMPILE_ENV		?=
$(1)_CC_COMPILE_MORE_FLAGS	?=
$(1)_CC_COMPILE_FLAGS		?= $$(CPPFLAGS) $$(CCFLAGS) $(2) $$(CFLAGS) $$($(1)_CC_COMPILE_MORE_FLAGS)
$(1)_CC_COMPILE			?= $$($(1)_CC_COMPILE_ENV) $$(CC) $$($(1)_CC_COMPILE_FLAGS) $$(CC_COMPILE_OUTPUT)
endef

# $(1) - the identifier of the module
# $(2) - a list of linker options
define ds-cc-build-program
$(1)_CC_BUILD_PROGRAM_ENV	?=
$(1)_CC_BUILD_PROGRAM_LIBS	?= $$(LDFLAGS) $$(LIBS)
$(1)_CC_BUILD_PROGRAM_FLAGS	?= $$(C_DEFAULT) $(2) $$(CFLAGS)
$(1)_CC_BUILD_PROGRAM_PRE	?=
$(1)_CC_BUILD_PROGRAM_POST	?=
$(1)_CC_BUILD_PROGRAM		?= $$($(1)_CC_BUILD_PROGRAM_ENV) $$(CC) \
					$$($(1)_CC_BUILD_PROGRAM_FLAGS) $$($(1)_CC_BUILD_PROGRAM_LIBS) \
					$$(CC_BUILD_OUTPUT) $$(@) \
					$$($(1)_CC_BUILD_PROGRAM_PRE) $$(^) $$($(1)_CC_BUILD_PROGRAM_POST)
endef

# $(1) - the identifier of the module
# $(2) - a list of linker options
define ds-cc-build-shared-library
$(1)_CC_BUILD_SHARED_LIBRARY_ENV	?=
$(1)_CC_BUILD_SHARED_LIBRARY_LIBS	?= $$(LDFLAGS) $$(LIBS)
$(1)_CC_BUILD_SHARED_LIBRARY_FLAGS	?= $$(C_DEFAULT) -fPIC -shared $(2) $$(CFLAGS)
$(1)_CC_BUILD_SHARED_LIBRARY_PRE	?=
$(1)_CC_BUILD_SHARED_LIBRARY_POST	?=
$(1)_CC_BUILD_SHARED_LIBRARY		?= $$($(1)_CC_BUILD_SHARED_LIBRARY_ENV) $$(CC) \
					$$($(1)_CC_BUILD_SHARED_LIBRARY_FLAGS) $$($(1)_CC_BUILD_SHARED_LIBRARY_LIBS) \
					$$(CC_BUILD_OUTPUT) $$(@) \
					$$($(1)_CC_BUILD_SHARED_LIBRARY_PRE) $$(^) $$($(1)_CC_BUILD_SHARED_LIBRARY_POST)
endef

#page

# $(1) - the identifier of the module
define ds-c-sources
$(1)_SRCDIR		?= $$(srcdir)/src
$(1)_BUILDDIR		?= $$(builddir)/objects.d
$(1)_PATTERNS		?= *.c
$(1)_PREREQUISITES	?=
$(1)_MAIN_SECTION	?= bin

$$(eval $$(call ds-srcdir,$(1),$$($(1)_SRCDIR)))
$$(eval $$(call ds-builddir,$(1),$$($(1)_BUILDDIR)))

vpath	%.h		$$($(1)_SRCDIR)
vpath	%.$$(OBJEXT)	$$($(1)_BUILDDIR)

$$(eval $$(call ds-cc-compile,$(1),-I$$($(1)_SRCDIR)))

$(1)_SOURCES	= $$(call ds-glob,$(1),$$(if $$($(1)_PATTERNS),$$($(1)_PATTERNS),*.c))
$(1)_OBJECTS	= $$(call ds-replace-dir,$$($(1)_BUILDDIR),$$($(1)_SOURCES:.c=.$$(OBJEXT)))

# This is needed because it is the interface to 'ds-module-no-install'
$(1)_TARGETS	+= $$($(1)_OBJECTS)

$(1)_CLEANFILES		+= $$($(1)_TARGETS)
$(1)_REALCLEANFILES	+= $$($(1)_CLEANFILES)

$$(eval $$(call ds-module-no-install,$(1),$$($(1)_MAIN_SECTION)))

$$($(1)_OBJECTS) : $$($(1)_BUILDDIR)/%.$$(OBJEXT) : $$($(1)_SRCDIR)/%.c $$($(1)_PREREQUISITES)
	$$($(1)_CC_COMPILE) $$(@) $$(<)
endef

#page

# $(1) - the identifier of the module
define ds-c-shared-library
ifeq ($$(ds_config_ENABLE_SHARED),yes)

$(1)_shared_library_BUILDDIR		?= $$(builddir)/libraries.d
$(1)_shared_library_OBJECTS		?= $$($(1)_OBJECTS)
$(1)_shared_library_ID			?= $(1)
$(1)_shared_library_LINK_ID		?=
$(1)_shared_library_MAIN_SECTION	?= bin

$(1)_shared_library_NAME	= lib$$($(1)_shared_library_ID).so
$(1)_shared_library_PATHNAME	= $$($(1)_shared_library_BUILDDIR)/$$($(1)_shared_library_NAME)
$(1)_shared_library_LINK_NAME	= lib$$($(1)_shared_library_LINK_ID).so
$(1)_shared_library_LINK_PATHNAME = $$($(1)_shared_library_BUILDDIR)/$$($(1)_shared_library_LINK_NAME)

$$(eval $$(call ds-cc-build-shared-library,$(1)_shared_library))
$$(eval $$(call ds-builddir,$(1)_shared_library,$$($(1)_shared_library_BUILDDIR)))

$(1)_shared_library_TARGETS	= $$($(1)_shared_library_PATHNAME) $$($(1)_shared_library_LINK_PATHNAME)
$(1)_shared_library_INSTLST	= $$($(1)_shared_library_PATHNAME)
$(1)_shared_library_INSTDIR	?= $(libdir)

$(1)_shared_library_CLEANFILES		+= $$($(1)_shared_library_TARGETS)
$(1)_shared_library_REALCLEANFILES	+= $$($(1)_shared_library_CLEANFILES)

$$(eval $$(call ds-module,$(1)_shared_library,$$($(1)_shared_library_MAIN_SECTION),LIB))

$$($(1)_shared_library_PATHNAME) : $$($(1)_shared_library_OBJECTS)
	$$(call ds-echo,'## ---------------------------------------------------------------------')
	$$(call ds-echo,'## Building shared library $$($(1)_shared_library_NAME)')
	$$($(1)_shared_library_CC_BUILD_SHARED_LIBRARY)
ifeq ($$(ds_config_ENABLE_STRIP),yes)
	$$(STRIP) $$(@)
endif
ifneq ($$(strip $$($(1)_shared_library_LINK_ID)),)
	cd $$($(1)_shared_library_BUILDDIR);\
	test -L $$($(1)_shared_library_LINK_NAME) || \
	$$(SYMLINK) $$($(1)_shared_library_NAME) $$($(1)_shared_library_LINK_NAME)
endif # $(1)_shared_library_LINK_ID = ""
	$$(call ds-echo,'## done.')

## ---------------------------------------------------------------------

ifneq ($$(strip $$($(1)_shared_library_LINK_ID)),)
$(1)_shared_library-install-aux:
	$$(call ds-install-directory,$(1)_shared_library)
	cd $$(DESTDIR)$$($(1)_shared_library_INSTDIR) ; \
	$$(SYMLINK) $$($(1)_shared_library_NAME) $$($(1)_shared_library_LINK_NAME)

$(1)_shared_library-uninstall-aux:
	$$(RM_FILE) $$(DESTDIR)$$(libdir)/$$($(1)_shared_library_LINK_NAME)

$(1)_shared_library-print-install-files-layout-aux:
	@echo $$($(1)_shared_library_INSTDIR)/$$($(1)_shared_library_LINK_NAME)

$(1)_shared_library-print-uninstall-files-script-aux:
	@echo $$(RM_FILE) $$($(1)_shared_library_INSTDIR)/$$($(1)_shared_library_LINK_NAME)

endif # $(1)_shared_library_LINK_ID = ""
endif # ds_config_ENABLE_SHARED = yes
endef

#page

# $(1) - the identifier of the module
define ds-c-library-preamble
ucl_LIBRARY_ID			= @ucl_LIBRARY_ID@
ucl_LIBRARY_LINK_ID		= @ucl_LIBRARY_LINK_ID@
ucl_INTERFACE_VERSION		= @ucl_INTERFACE_VERSION@
ucl_INTERFACE_MAJOR_VERSION	= @ucl_INTERFACE_MAJOR_VERSION@
ucl_INTERFACE_MINOR_VERSION	= @ucl_INTERFACE_MINOR_VERSION@
ucl_SHARED_LIBRARY_ID		= @ucl_SHARED_LIBRARY_ID@
ucl_SHARED_LIBRARY_LINK_ID	= @ucl_SHARED_LIBRARY_LINK_ID@
ucl_SHARED_LIBRARY_NAME		= @ucl_SHARED_LIBRARY_NAME@
ucl_SHARED_LIBRARY_LINK_NAME	= @ucl_SHARED_LIBRARY_LINK_NAME@
ucl_STATIC_LIBRARY_ID		= @ucl_STATIC_LIBRARY_ID@
ucl_STATIC_LIBRARY_NAME		= @ucl_STATIC_LIBRARY_NAME@
endef

# $(1) - the identifier of the module
define ds-c-static-library
ifeq ($$(ds_config_ENABLE_STATIC),yes)

$(1)_static_library_BUILDDIR		?= $$(builddir)/libraries.d
$(1)_static_library_OBJECTS		?= $$($(1)_OBJECTS)
$(1)_static_library_ID			?= $(1)
$(1)_static_library_MAIN_SECTION	?= dev

$(1)_static_library_NAME	= lib$$($(1)_static_library_ID).a
$(1)_static_library_PATHNAME	= $$($(1)_static_library_BUILDDIR)/$$($(1)_static_library_NAME)

$$(eval $$(call ds-builddir,$(1)_static_library,$$($(1)_static_library_BUILDDIR)))

$(1)_static_library_TARGETS	= $$($(1)_static_library_PATHNAME)
$(1)_static_library_INSTLST	= $$($(1)_static_library_TARGETS)
$(1)_static_library_INSTDIR	?= $(libdir)

$$(eval $$(call ds-default-clean-files-variables,$(1)_static_library))
$$(eval $$(call ds-module,$(1)_static_library,$$($(1)_static_library_MAIN_SECTION),LIB))

$$($(1)_static_library_PATHNAME) : $$($(1)_static_library_OBJECTS)
	$$(call ds-echo,'## ---------------------------------------------------------------------')
	$$(call ds-echo,'## Building static library $$($(1)_static_library_NAME)')
	$$(AR) $$(@) $$(^)
	-($$(RANLIB) $$(@) || true) >/dev/null 2>&1
	$$(call ds-echo,'## done.')

endif # ds_config_ENABLE_STATIC = yes
endef

# $(1) - the identifier of the module
define ds-c-library
$$(eval $$(call ds-c-sources,$(1)))
$$(eval $$(call ds-c-library-preamble,$(1)))
$$(eval $$(call ds-c-static-library,$(1)))
$$(eval $$(call ds-c-shared-library,$(1)))
endef

#page

# $(1) - the identifier of the module
define ds-c-single-program
$(1)_program_BUILDDIR		?= $$(builddir)/programs.d
$(1)_program_OBJECTS		?= $$($(1)_OBJECTS)
$(1)_program_PREFIX		?=
$(1)_program_ENV		?=
$(1)_program_MAIN_SECTION	?= bin

$(1)_program_NAME	= $$($(1)_program_PREFIX)$(2)
$(1)_program_PATHNAME	= $$($(1)_program_BUILDDIR)/$$($(1)_program_NAME)

$$(eval $$(call ds-cc-build-program,$(1)_program))
$$(eval $$(call ds-builddir,$(1)_program,$$(if $$($(1)_program_BUILDDIR),$$($(1)_program_BUILDDIR),$$(builddir)/programs.d)))

$(1)_program_TARGETS	+= $$($(1)_program_PATHNAME)
$(1)_program_INSTLST	= $$($(1)_program_PATHNAME)
$(1)_program_INSTDIR	?= $(pkglibexecdir)

$$(eval $$(call ds-default-clean-files-variables,$(1)_program))
$$(eval $$(call ds-module,$(1)_program,$$($(1)_program_MAIN_SECTION),LIB))

$$($(1)_program_PATHNAME) : $$($(1)_program_OBJECTS)
	$$($(1)_program_CC_BUILD_PROGRAM)
ifeq ($$(ds_config_ENABLE_STRIP),yes)
	$$(STRIP) $$(@)
endif

.PHONY: run-$(1)

run-$(1): $(1)_program-all
	$$($(1)_program_ENV) $$($(1)_program_PATHNAME)
endef


## ---------------------------------------------------------------------

define ds-c-single-program-no-install

# $(1) is the identifier of the module
# $(2) is the name of the program
$(1)_program_BUILDDIR		?= $$(builddir)/programs.d
$(1)_program_OBJECTS		?= $$($(1)_OBJECTS)
$(1)_program_PREFIX		?=
$(1)_program_ENV		?=
$(1)_program_MAIN_SECTION	?= bin

$(1)_program_NAME	= $$($(1)_program_PREFIX)$(2)
$(1)_program_PATHNAME	= $$($(1)_program_BUILDDIR)/$$($(1)_program_NAME)

$$(eval $$(call ds-builddir,$(1)_program,$$($(1)_program_BUILDDIR)))
$$(eval $$(call ds-cc-build-program,$(1)_program))

$(1)_program_TARGETS	= $$($(1)_program_PATHNAME)

$$(eval $$(call ds-default-clean-files-variables,$(1)_program))
$$(eval $$(call ds-module-no-install,$(1)_program,$$($(1)_program_MAIN_SECTION)))

$$($(1)_program_PATHNAME) : $$($(1)_program_OBJECTS)
	$$($(1)_program_CC_BUILD_PROGRAM)
ifeq ($$(ds_config_ENABLE_STRIP),yes)
	$$(STRIP) $$(@)
endif

.PHONY: run-$(1)

run-$(1): $(1)_program-all
	$$($(1)_program_ENV) $$($(1)_program_PATHNAME)
endef

#page

# $(1) - the identifier of the module
define ds-c-programs
$(1)_programs_SRCDIR		?= $$($(1)_BUILDDIR)
$(1)_programs_BUILDDIR		?= $$(builddir)/programs.d
$(1)_programs_OBJECTS		?= $$($(1)_OBJECTS)
$(1)_programs_PREFIX		?=
$(1)_programs_ENV		?=
$(1)_programs_MAIN_SECTION	?= bin

$(1)_programs_NAMES	= $$(addprefix $$($(1)_programs_PREFIX),$$(notdir $$($(1)_programs_OBJECTS:.$(OBJEXT)=)))
$(1)_programs_PATHNAMES	= $$(addprefix $$($(1)_programs_BUILDDIR)/,$$($(1)_programs_NAMES))

$$(eval $$(call ds-builddir,$(1)_programs,$$($(1)_programs_BUILDDIR)))
$$(eval $$(call ds-cc-build-program,$(1)_programs))

$(1)_programs_TARGETS	= $$($(1)_programs_PATHNAMES)
$(1)_programs_INSTLST	= $$($(1)_programs_PATHNAMES)
$(1)_programs_INSTDIR	?= $(pkglibexecdir)

$$(eval $$(call ds-default-clean-files-variables,$(1)_programs))
$$(eval $$(call ds-module,$(1)_programs,$$($(1)_programs_MAIN_SECTION)))

$$($(1)_programs_PATHNAMES) : $$($(1)_programs_BUILDDIR)/$$($(1)_programs_PREFIX)% : $$($(1)_programs_SRCDIR)/%.$(OBJEXT)
	$$($(1)_programs_CC_BUILD_PROGRAM)
ifeq ($$(ds_config_ENABLE_STRIP),yes)
	$$(STRIP) $$(@)
endif

.PHONY: run-$(1) $$(addprefix run-,$$($(1)_programs_NAMES))

run-$(1): $$(addprefix run-,$$($(1)_programs_NAMES))

$$(addprefix run-,$$($(1)_programs_NAMES)): run-% : $$($(1)_programs_BUILDDIR)/%
	$$($(1)_programs_ENV) $$(<)

endef

## ---------------------------------------------------------------------

define ds-c-programs-no-install

# $(1) is the identifier of the module
$(1)_programs_SRCDIR		?= $$($(1)_BUILDDIR)
$(1)_programs_OBJECTS		?= $$($(1)_OBJECTS)
$(1)_programs_BUILDDIR		?= $$(builddir)/programs.d
$(1)_programs_PREFIX		?=
$(1)_programs_ENV		?=
$(1)_programs_MAIN_SECTION	?= bin

$(1)_programs_NAMES	= $$(addprefix $$($(1)_programs_PREFIX),$$(notdir $$($(1)_programs_OBJECTS:.$(OBJEXT)=)))
$(1)_programs_PATHNAMES	= $$(addprefix $$($(1)_programs_BUILDDIR)/,$$($(1)_programs_NAMES))

$$(eval $$(call ds-builddir,$(1)_programs,$$($(1)_programs_BUILDDIR)))
$$(eval $$(call ds-cc-build-program,$(1)_programs))

$(1)_programs_TARGETS	= $$($(1)_programs_PATHNAMES)

$$(eval $$(call ds-default-clean-files-variables,$(1)_programs))
$$(eval $$(call ds-module-no-install,$(1)_programs,$$($(1)_programs_MAIN_SECTION)))

$$($(1)_programs_PATHNAMES) : $$($(1)_programs_BUILDDIR)/$$($(1)_programs_PREFIX)% : $$($(1)_programs_SRCDIR)/%.$(OBJEXT)
	$$($(1)_programs_CC_BUILD_PROGRAM)
ifeq ($$(ds_config_ENABLE_STRIP),yes)
	$$(STRIP) $$(@)
endif

.PHONY: run-$(1) $$(addprefix run-,$$($(1)_programs_NAMES))

run-$(1): $$(addprefix run-,$$($(1)_programs_NAMES))

$$(addprefix run-,$$($(1)_programs_NAMES)): run-% : $$($(1)_programs_BUILDDIR)/%
	$$($(1)_programs_ENV) $$(<)

endef

#page

# $(1) - the identifier of the module
define ds-c-example-programs
$(1)_examples_SRCDIR		?= $$(srcdir)/examples
$(1)_examples_BUILDDIR		?= $$(builddir)/examples.d
$(1)_examples_MAIN_SECTION	?= examples

$$(eval $$(call ds-c-sources,$(1)_examples))

$(1)_examples_programs_BUILDDIR		?= $$($(1)_examples_BUILDDIR)
$(1)_examples_programs_MAIN_SECTION	= examples

$$(eval $$(call ds-c-programs-no-install,$(1)_examples))

endef

#page

# $(1) - is the identifier of the module
define ds-c-test-programs
$(1)_tests_SRCDIR		?= $$(srcdir)/tests
$(1)_tests_BUILDDIR		?= $$(builddir)/tests.d
$(1)_tests_MAIN_SECTION		?= test

$$(eval $$(call ds-c-sources,$(1)_tests))

$(1)_tests_programs_BUILDDIR		?= $$($(1)_tests_BUILDDIR)
$(1)_tests_programs_MAIN_SECTION	?= test

$$(eval $$(call ds-c-programs-no-install,$(1)_tests))

$$($(1)_tests_programs_MAIN_SECTION): run-$(1)_tests

endef

#page

# $(1) - the identifier of the module
# $(2) - the source directory
# $(3) - patterns to select the header files
define ds-h-files-installer
$(1)_c_headers_SRCDIR		?= $(2)
$(1)_c_headers_PATTERNS		?= $(3)
$(1)_c_headers_MAIN_SECTION	?= dev

$$(eval $$(call ds-srcdir,$(1),$$($(1)_c_headers_SRCDIR)))

$(1)_c_headers_INSTLST	= \
	$$(call ds-glob,$(1),$$(if $$($(1)_c_headers_PATTERNS),$$($(1)_c_headers_PATTERNS),*.h))
$(1)_c_headers_INSTDIR	?= $$(pkgincludedir)

$$(eval $$(call ds-module,$(1)_c_headers,$$($(1)_c_headers_MAIN_SECTION)))

endef

#page
## --------------------------------------------------------------------
## Generic packaging stuff.
## --------------------------------------------------------------------

ds_archive_NAME		?= $(PACKAGE_NAME)
ds_archive_VERSION	?= $(PACKAGE_VERSION)

ifneq ($(strip $(BUILD_VERSION)),)
ds_archive_BUILD_VERSION= $(BUILD_VERSION)
else
ds_archive_BUILD_VERSION= 1mm
endif

ifneq ($(strip $(ARCHIVE_ARCH)),)
ds_archive_ARCH		= $(ARCHIVE_ARCH)
else
ds_archive_ARCH		= \
	$(firstword $(subst -, ,$(shell $(infrastructuredir)/config.guess)))
endif

ds_archive_NAMETAIL	= $(ds_archive_VERSION)-$(ds_archive_ARCH)-$(ds_archive_BUILD_VERSION)
ds_archive_bin_PREFIX	= $(ds_archive_NAME)-$(ds_archive_NAMETAIL)
ds_archive_doc_PREFIX	= $(ds_archive_NAME)-doc-$(ds_archive_NAMETAIL)
ds_archive_dev_PREFIX	= $(ds_archive_NAME)-dev-$(ds_archive_NAMETAIL)
ds_archive_full_PREFIX	= $(ds_archive_NAME)-full-$(ds_archive_NAMETAIL)

ifeq ($(ds_config_USE_SUDO),yes)
ds_archive_SUDO		= $(SUDO)
endif

#page
define ds-source-distribution
ds_dist_TMPDIR		?= $$(TMPDIR)/$$(PKG_ID)
ds_dist_ARCHIVE		?= $$(PKG_ID)-src.tar.bz2
ds_dist_DESTDIR		= $$(builddir)/dist.d

ds_dist_REPOSITORY	?= /usr/local/src
ds_dist_PACKAGE_SECTION	?= local
ds_dist_PACKAGE_DEST	= $$(ds_dist_REPOSITORY)/$$(ds_dist_PACKAGE_SECTION)

.PHONY: dist dist-store

dist:
	-test -d $$(ds_dist_DESTDIR) || $$(MKDIR) $$(ds_dist_DESTDIR)
	$$(RM_SILENT) $$(ds_dist_TMPDIR)
	$$(RM_SILENT) $$(TMPDIR)/$$(ds_dist_ARCHIVE)
	$$(MKDIR) $$(ds_dist_TMPDIR)
	$$(TAR) \
		--directory=$$(srcdir) --create --file=- --dereference		\
		--exclude=RCS                   --exclude=CVS                   \
		--exclude=.git			--exclude=.git\*		\
		--exclude=archives              --exclude=\*.ps			\
		--exclude=\*.dvi                --exclude=tmp			\
		--exclude=\*.gz                 --exclude=\*.tar                \
		--exclude=\*.so                 --exclude=\*.o			\
		--exclude=\*.a                  --exclude=\*.rpm                \
		--exclude=\*.deb                --exclude=.emacs\*		\
		--exclude=\*~                   --exclude=TAGS                  \
		--exclude=config.log            --exclude=config.status         \
		--exclude=config.cache          --exclude=Makefile              \
		--exclude=autom4te.cache	--exclude="{arch}"              \
		--exclude=.arch-ids		--exclude=\+\+\*                \
		--exclude=\=\*                                                  \
		. | $$(TAR) --directory=$$(ds_dist_TMPDIR) --extract --file=-
	$$(TAR) --directory=$$(TMPDIR) --verbose \
		--create --bzip2 --file=$$(ds_dist_DESTDIR)/$$(ds_dist_ARCHIVE) $$(PKG_ID)
	$$(RM_SILENT) $$(ds_dist_TMPDIR)

dist-store:
	$$(MV) $$(ds_dist_DESTDIR)/$$(ds_dist_ARCHIVE) $$(ds_dist_PACKAGE_DEST)

endef

#page
define ds-binary-distribution
ds_bindist_TMPDIR	= $$(TMPDIR)/$$(PKG_ID)
ds_bindist_DESTDIR	= $$(builddir)/bindist.d

ds_bindist_bin_ARCHIVE	= $$(ds_archive_bin_PREFIX).tar.gz
ds_bindist_doc_ARCHIVE	= $$(ds_archive_doc_PREFIX).tar.gz
ds_bindist_dev_ARCHIVE	= $$(ds_archive_dev_PREFIX).tar.gz

.PHONY: bindist         bindist-bin         bindist-doc         bindist-dev
.PHONY: bindist-install bindist-bin-install bindist-doc-install bindist-dev-install

bindist:		$$(addprefix bindist-,         $$(ds_INCLUDED_RULES))
bindist-install:	$$(addprefix bindist-install-, $$(ds_INCLUDED_RULES))

bindist-bin:
	$$(call ds-bindist-make-package,bin-install,$$(ds_bindist_bin_ARCHIVE))

bindist-doc:
	$$(call ds-bindist-make-package,doc-install,$$(ds_bindist_doc_ARCHIVE))

bindist-dev:
	$$(call ds-bindist-make-package,dev-install,$$(ds_bindist_dev_ARCHIVE))

bindist-install-bin:
	$$(call ds-bindist-install,$$(ds_bindist_bin_ARCHIVE))

bindist-install-doc:
	$$(call ds-bindist-install,$$(ds_bindist_doc_ARCHIVE))

bindist-install-dev:
	$$(call ds-bindist-install,$$(ds_bindist_dev_ARCHIVE))

ds_bindist_ARCHIVE_FULL	= $$(ds_archive_FULL_PREFIX).tar.gz

.PHONY: bindist-full bindist-install-full

bindist-full:
	$$(call ds-bindist-make-package,\
		$$(addsuffix -install,$$(ds_INCLUDED_RULES)),\
		$$(ds_bindist_full_ARCHIVE))

bindist-install-full:
	$$(call ds-bindist-install,$$(ds_bindist_full_ARCHIVE))

endef

define ds-bindist-make-package
test -d $(ds_bindist_DESTDIR) || $(MKDIR) $(ds_bindist_DESTDIR)
$(RM_SILENT) $(ds_bindist_TMPDIR)
$(MAKE) $(1) DESTDIR=$(ds_bindist_TMPDIR)
$(TAR) --directory=$(ds_bindist_TMPDIR) \
	--create --gzip --verbose \
	--file=$(ds_bindist_DESTDIR)/$(strip $(2)) .
$(RM_SILENT) $(ds_bindist_TMPDIR)
endef

define ds-bindist-install
$(TAR) --directory=/						\
	--extract --gzip --verbose				\
	--no-overwrite-dir --no-same-owner --same-permissions	\
	--file=$(ds_bindist_DESTDIR)/$(1)
endef

#page
## ---------------------------------------------------------------------
## Slackware packaging.
## ---------------------------------------------------------------------

# define ds-slackware-get-installed-package
# $(shell ls $(ds_slackware_REGISTRY) | grep '$(1)-[0-9]' | head)
# endef
define ds-slackware-get-installed-package
$(notdir $(firstword $(wildcard $(ds_slackware_REGISTRY)/$(1)-[0-9]*)))
endef

define ds-private-slackware-targets

ifeq ($(1),bin)
ds_slackware_$(1)_INSTALLED_PACKAGE_SPEC	= $$(PACKAGE_NAME)
else
ds_slackware_$(1)_INSTALLED_PACKAGE_SPEC	= $$(PACKAGE_NAME)-$(1)
endif

.PHONY: private-slackware-make-$(1)   private-slackware-install-$(1)
.PHONY: private-slackware-remove-$(1) private-slackware-upgrade-$(1)

private-slackware-make-$(1): slackware-builddir
	@$$(MAKE) private-slackware-clean-builddir-$(1)
	$$(ds_archive_SUDO) $$(MAKE) $(1)-install \
		DESTDIR=$$(ds_slackware_PACKAGE_TOP_BUILDDIR)
	$$(ds_archive_SUDO) $$(INSTALL_DIR) \
		$$(ds_slackware_PACKAGE_BUILDDIR)/install
	$$(ds_archive_SUDO) $$(INSTALL_DATA)					\
		$$(ds_meta_builddir)/slackware/$(1)/slack-desc			\
		$$(ds_slackware_PACKAGE_BUILDDIR)/install
	$$(ds_archive_SUDO) $$(MAKE) slackware-aux-$(1)
	cd $$(ds_slackware_PACKAGE_BUILDDIR);					\
	$$(ds_slackware_MAKEPKG) $$(ds_slackware_$(1)_PACKAGE_NAME);		\
	$$(ds_archive_SUDO) $$(MV)						\
		$$(ds_slackware_$(1)_PACKAGE_NAME) $$(ds_slackware_BUILDDIR)

private-slackware-install-$(1):
	cd $$(ds_slackware_BUILDDIR); \
	$$(ds_slackware_INSTALLPKG) $$(ds_slackware_$(1)_PACKAGE_NAME)

private-slackware-remove-$(1):
	$$(ds_slackware_REMOVEPKG) $$(ds_archive_$(1)_PREFIX)

private-slackware-upgrade-$(1):
	cd $$(ds_slackware_BUILDDIR); $$(ds_slackware_UPGRADEPKG) \
		$$(call ds-slackware-get-installed-package,$$(ds_slackware_$(1)_INSTALLED_PACKAGE_SPEC))%$$(ds_slackware_$(1)_PACKAGE_NAME)

private-slackware-clean-builddir-$(1):
	-$$(ds_archive_SUDO) $$(RM) $$(ds_slackware_$(1)_PACKAGE_NAME)

.PHONY: slackware-aux-$(1)

slackware-aux-$(1):

.PHONY: slackware-make-$(1)   slackware-install-$(1)
.PHONY: slackware-remove-$(1) slackware-upgrade-$(1)

slackware-make-$(1):
	$$(MAKE) private-slackware-make-$(1)	$$(ds_slackware_STANDARD_ENV)
	$$(MAKE) slackware-clean-top-builddir	$$(ds_slackware_STANDARD_ENV)

slackware-install-$(1):
	$$(MAKE) private-slackware-install-$(1)	$$(ds_slackware_STANDARD_ENV)

slackware-remove-$(1):
	$$(MAKE) private-slackware-remove-$(1)	$$(ds_slackware_STANDARD_ENV)

slackware-upgrade-$(1):
	$$(MAKE) private-slackware-upgrade-$(1)	$$(ds_slackware_STANDARD_ENV)

.PHONY: local-slackware-make-$(1)   local-slackware-install-$(1)
.PHONY: local-slackware-remove-$(1) local-slackware-upgrade-$(1)

local-slackware-make-$(1):
	$$(MAKE) private-slackware-make-$(1)	$$(ds_slackware_LOCAL_ENV)
	$$(MAKE) slackware-clean-top-builddir	$$(ds_slackware_LOCAL_ENV)

local-slackware-install-$(1):
	$$(MAKE) private-slackware-install-$(1)	$$(ds_slackware_LOCAL_ENV)

local-slackware-remove-$(1):
	$$(MAKE) private-slackware-remove-$(1)	$$(ds_slackware_LOCAL_ENV)

local-slackware-upgrade-$(1):
	$$(MAKE) private-slackware-upgrade-$(1)	$$(ds_slackware_LOCAL_ENV)

endef

define ds-slackware-distribution

ds_slackware_bin_PACKAGE_NAME	= $$(ds_archive_bin_PREFIX).tgz
ds_slackware_doc_PACKAGE_NAME	= $$(ds_archive_doc_PREFIX).tgz
ds_slackware_dev_PACKAGE_NAME	= $$(ds_archive_dev_PREFIX).tgz
ds_slackware_PACKAGE_TOP_BUILDDIR = $$(TMPDIR)/$$(PKG_ID)
ds_slackware_PACKAGE_BUILDDIR	?=
ds_slackware_BUILDDIR		= $$(abspath $$(builddir)/slackware.d)
ds_slackware_REGISTRY		?=
ds_slackware_REGISTRY_DIR	= /var/log/packages
ds_slackware_ENV		?=

ds_slackware_MAKEPKG_PROGRAM	?= @ds_slackware_MAKEPKG_PROGRAM@
ifeq ($$(ds_config_SLACKWARE_CHOWN),yes)
ds_slackware_MAKEPKG_FLAGS	?= --chown y
else
ds_slackware_MAKEPKG_FLAGS	?= --chown n
endif
ds_slackware_MAKEPKG		= $$(ds_slackware_ENV) $$(ds_archive_SUDO) $$(ds_slackware_MAKEPKG_PROGRAM) $$(ds_slackware_MAKEPKG_FLAGS)

ds_slackware_INSTALLPKG_PROGRAM	?= @ds_slackware_INSTALLPKG_PROGRAM@
ds_slackware_INSTALLPKG_FLAGS	?=
ds_slackware_INSTALLPKG	= $$(ds_slackware_ENV) $$(ds_archive_SUDO) $$(ds_slackware_INSTALLPKG_PROGRAM) $$(ds_slackware_INSTALLPKG_FLAGS)

ds_slackware_REMOVEPKG_PROGRAM	?= @ds_slackware_REMOVEPKG_PROGRAM@
ds_slackware_REMOVEPKG_FLAGS	?=
ds_slackware_REMOVEPKG		= $$(ds_slackware_ENV) $$(ds_archive_SUDO) $$(ds_slackware_REMOVEPKG_PROGRAM) $$(ds_slackware_REMOVEPKG_FLAGS)

ds_slackware_UPGRADEPKG_PROGRAM	?= @ds_slackware_UPGRADEPKG_PROGRAM@
ds_slackware_UPGRADEPKG_FLAGS	?= --verbose --reinstall
ds_slackware_UPGRADEPKG	= $$(ds_slackware_ENV) $$(ds_archive_SUDO) $$(ds_slackware_UPGRADEPKG_PROGRAM) $$(ds_slackware_UPGRADEPKG_FLAGS)

ds_local_slackware_MAKEPKG_PROGRAM	= @ds_local_slackware_MAKEPKG_PROGRAM@
ds_local_slackware_INSTALLPKG_PROGRAM	= @ds_local_slackware_INSTALLPKG_PROGRAM@
ds_local_slackware_REMOVEPKG_PROGRAM	= @ds_local_slackware_REMOVEPKG_PROGRAM@
ds_local_slackware_UPGRADEPKG_PROGRAM	= @ds_local_slackware_UPGRADEPKG_PROGRAM@

## ---------------------------------------------------------------------

.PHONY: slackware-builddir slackware-top-builddir
.PHONY: slackware-clean slackware-clean-top-builddir slackware-clean-builddir

slackware-builddir:
	-test -d $$(ds_slackware_BUILDDIR) || \
		$$(MKDIR) $$(ds_slackware_BUILDDIR)

slackware-top-builddir:
	-test -d $$(ds_slackware_PACKAGE_TOP_BUILDDIR) || \
		$$(MKDIR) $$(ds_slackware_PACKAGE_TOP_BUILDDIR)

slackware-clean: slackware-clean-top-builddir slackware-clean-builddir

slackware-clean-top-builddir:
	-$$(ds_archive_SUDO) $$(RM) $$(ds_slackware_PACKAGE_TOP_BUILDDIR)

slackware-clean-builddir:
	-$$(ds_archive_SUDO) $$(RM) $$(ds_slackware_BUILDDIR)/*.tgz

## ---------------------------------------------------------------------

# PATH is  required when doing  upgrade, because upgradepkg  invokes the
# other scripts.
ds_slackware_STANDARD_ENV	= \
PATH=/sbin:$$(PATH)							\
ds_slackware_PACKAGE_BUILDDIR=$$(ds_slackware_PACKAGE_TOP_BUILDDIR)	\
ds_slackware_REGISTRY=$$(ds_slackware_REGISTRY_DIR)

.PHONY: slackware slackware-install slackware-remove slackware-upgrade

slackware:		$(addprefix slackware-make-,	  $(ds_INCLUDED_RULES))
slackware-install:	$(addprefix slackware-install-,	  $(ds_INCLUDED_RULES))
slackware-remove:	$(addprefix slackware-remove-,	  $(ds_INCLUDED_RULES))
slackware-upgrade:	$(addprefix slackware-upgrade-,	  $(ds_INCLUDED_RULES))

## --------------------------------------------------------------------

# PATH is  required when doing  upgrade, because upgradepkg  invokes the
# other scripts.
ds_slackware_LOCAL_ENV	= \
PATH=$$(prefix)/sbin:/sbin:$$(PATH)						\
ds_slackware_PACKAGE_BUILDDIR=$$(ds_slackware_PACKAGE_TOP_BUILDDIR)/$$(prefix)	\
ds_slackware_REGISTRY=$$(prefix)$$(ds_slackware_REGISTRY_DIR)			\
ds_slackware_ENV=ROOT=$$(prefix)						\
ds_slackware_MAKEPKG_PROGRAM=$$(ds_local_slackware_MAKEPKG_PROGRAM)		\
ds_slackware_INSTALLPKG_PROGRAM=$$(ds_local_slackware_INSTALLPKG_PROGRAM)	\
ds_slackware_REMOVEPKG_PROGRAM=$$(ds_local_slackware_REMOVEPKG_PROGRAM)		\
ds_slackware_UPGRADEPKG_PROGRAM=$$(ds_local_slackware_UPGRADEPKG_PROGRAM)

.PHONY: local-slackware        local-slackware-install
.PHONY: local-slackware-remove local-slackware-upgrade

local-slackware:	$(addprefix local-slackware-make-,	  $(ds_INCLUDED_RULES))
local-slackware-install:$(addprefix local-slackware-install-,	  $(ds_INCLUDED_RULES))
local-slackware-remove:	$(addprefix local-slackware-remove-,	  $(ds_INCLUDED_RULES))
local-slackware-upgrade:$(addprefix local-slackware-upgrade-,	  $(ds_INCLUDED_RULES))

## --------------------------------------------------------------------

$$(eval $$(call ds-private-slackware-targets,bin))
$$(eval $$(call ds-private-slackware-targets,doc))
$$(eval $$(call ds-private-slackware-targets,dev))

endef


### end of file
# Local Variables:
# mode: makefile
# End:
