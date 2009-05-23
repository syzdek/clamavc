#
#   Clam Antivirus Client Library
#   Copyright (C) 2009 Alaska Communications Systems Group, Inc
#
#   @ACS_LICENSE_HEADER_START@
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program; if not, write to the Free Software Foundation, Inc.,
#   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
#   @ACS_LICENSE_HEADER_END@
#
#   @configure_input@
#
#   Makefile.am - automate compiling on a unix platform
#

# Sub directories
SUBDIRS					=
DIST_SUBDIRS				=


# global flags
AM_LDFLAGS				= -L$(builddir)/lib
AM_LIBS					=
AM_CFLAGS				= -O2 $(CFLAGS_WARNINGS)
AM_CPPFLAGS				= -I$(top_srcdir)/include \
					  -I$(top_builddir)/include
AM_LIBTOOLFLAGS				=
ACLOCAL_AMFLAGS				= -I m4
DEFS					= @DEFS@


# items to build
noinst_PROGRAMS				= src/clamavc-contscan \
					  src/clamavc-instream \
					  src/clamavc-multiscan \
					  src/clamavc-reload \
					  src/clamavc-scan \
					  src/clamavc-shutdown
include_HEADERS				= include/clamavc.h
lib_LTLIBRARIES				= lib/libclamavc.la
man_MANS				=


# macros for data/invert-file
data_invert_file_DEPENDENCIES		= Makefile
data_invert_file_CPPFLAGS		= -DPROGRAM_NAME="\"invert-file\"" $(AM_CPPFLAGS)
data_invert_file_CFLAGS			=
data_invert_file_LDFLAGS		=
data_invert_file_LDADD			=
data_invert_file_SOURCES		= data/invert-file.c


# macros for lib/libclamavc.la
lib_libclamavc_la_DEPENDENCIES		= Makefile lib/libclamavc.sym
lib_libclamavc_la_CFLAGS		= $(AM_CFLAGS)
lib_libclamavc_la_LIBADD		= $(AM_LIBS)
lib_libclamavc_la_LDFLAGS		= -rpath '$(libdir)' \
					  -version-info $(LIB_VERSION_INFO) \
					  $(AM_LDFLAGS) \
					  -export-symbols $(srcdir)/lib/libclamavc.sym
lib_libclamavc_la_SOURCES		= include/clamavc.h lib/libclamavc.c


# macros for src/clamavc-contscan
src_clamavc_contscan_DEPENDENCIES	= Makefile lib/libclamavc.la
src_clamavc_contscan_CPPFLAGS		= -DPROGRAM_NAME="\"clamavc-contscan\"" $(AM_CPPFLAGS)
src_clamavc_contscan_CFLAGS		= $(AM_CFLAGS)
src_clamavc_contscan_LDFLAGS		= $(AM_LDFLAGS)
src_clamavc_contscan_LDADD		= $(AM_LDADD) lib/libclamavc.la
src_clamavc_contscan_SOURCES		= include/clamavc.h \
					  src/clamavc-common.c \
					  src/clamavc-common.h \
					  src/clamavc-contscan.c


# macros for src/clamavc-instream
src_clamavc_instream_DEPENDENCIES	= Makefile lib/libclamavc.la
src_clamavc_instream_CPPFLAGS		= -DPROGRAM_NAME="\"clamavc-instream\"" $(AM_CPPFLAGS)
src_clamavc_instream_CFLAGS		= $(AM_CFLAGS)
src_clamavc_instream_LDFLAGS		= $(AM_LDFLAGS)
src_clamavc_instream_LDADD		= $(AM_LDADD) lib/libclamavc.la
src_clamavc_instream_SOURCES		= include/clamavc.h \
					  src/clamavc-common.c \
					  src/clamavc-common.h \
					  src/clamavc-instream.c


# macros for src/clamavc-multiscan
src_clamavc_multiscan_DEPENDENCIES	= Makefile lib/libclamavc.la
src_clamavc_multiscan_CPPFLAGS		= -DPROGRAM_NAME="\"clamavc-multiscan\"" $(AM_CPPFLAGS)
src_clamavc_multiscan_CFLAGS		= $(AM_CFLAGS)
src_clamavc_multiscan_LDFLAGS		= $(AM_LDFLAGS)
src_clamavc_multiscan_LDADD		= $(AM_LDADD) lib/libclamavc.la
src_clamavc_multiscan_SOURCES		= include/clamavc.h \
					  src/clamavc-common.c \
					  src/clamavc-common.h \
					  src/clamavc-multiscan.c


# macros for src/clamavc-reload
src_clamavc_reload_DEPENDENCIES		= Makefile lib/libclamavc.la
src_clamavc_reload_CPPFLAGS		= -DPROGRAM_NAME="\"clamavc-reload\"" $(AM_CPPFLAGS)
src_clamavc_reload_CFLAGS		= $(AM_CFLAGS)
src_clamavc_reload_LDFLAGS		= $(AM_LDFLAGS)
src_clamavc_reload_LDADD		= $(AM_LDADD) lib/libclamavc.la
src_clamavc_reload_SOURCES		= include/clamavc.h \
					  src/clamavc-common.c \
					  src/clamavc-common.h \
					  src/clamavc-reload.c


# macros for src/clamavc-scan
src_clamavc_scan_DEPENDENCIES		= Makefile lib/libclamavc.la
src_clamavc_scan_CPPFLAGS		= -DPROGRAM_NAME="\"clamavc-scan\"" $(AM_CPPFLAGS)
src_clamavc_scan_CFLAGS			= $(AM_CFLAGS)
src_clamavc_scan_LDFLAGS		= $(AM_LDFLAGS)
src_clamavc_scan_LDADD			= $(AM_LDADD) lib/libclamavc.la
src_clamavc_scan_SOURCES		= include/clamavc.h \
					  src/clamavc-common.c \
					  src/clamavc-common.h \
					  src/clamavc-scan.c


# macros for src/clamavc-shutdown
src_clamavc_shutdown_DEPENDENCIES	= Makefile lib/libclamavc.la
src_clamavc_shutdown_CPPFLAGS		= -DPROGRAM_NAME="\"clamavc-shutdown\"" $(AM_CPPFLAGS)
src_clamavc_shutdown_CFLAGS		= $(AM_CFLAGS)
src_clamavc_shutdown_LDFLAGS		= $(AM_LDFLAGS)
src_clamavc_shutdown_LDADD		= $(AM_LDADD) lib/libclamavc.la
src_clamavc_shutdown_SOURCES		= include/clamavc.h \
					  src/clamavc-common.c \
					  src/clamavc-common.h \
					  src/clamavc-shutdown.c


# miscellaneous lists
CLEANFILES				= $(builddir)/a.out   $(srcdir)/a.out \
					  $(builddir)/*/a.out $(srcdir)/*/a.out \
					  $(builddir)/doc/otadm.conf-example \
					  config.h.in~ $(srcdir)/config.h.in~ \
					  stamp-vti $(srcdir)/stamp-vti \
					  $(man_MANS) \
					  @PACKAGE_TARNAME@-*.tar.* \
					  @PACKAGE_TARNAME@-*.zip \
					  $(EXTRA_PROGRAMS)
EXTRA_DIST				= data/Makefile \
					  data/README \
					  data/infected.tar.gz.bin \
					  data/invert-file.c \
					  data/non-infected.tar.gz.bin \
					  lib/libclamavc.sym
EXTRA_PROGRAMS				= data/invert-file
EXTRA_TEXINFOS				=
TESTS					=
XFAIL_TESTS				=


# substitution routine
do_subst = sed \
		-e 's,[@]SHELL[@],@SHELL@,g' \
		-e 's,[@]PACKAGE_VERSION[@],@PACKAGE_VERSION@,g' \
		-e 's,[@]PACKAGE_NAME[@],@PACKAGE_NAME@,g' \
		-e 's,[@]PACKAGE_TARNAME[@],@PACKAGE_TARNAME@,g' \
		-e 's,[@]PACKAGE_BUGREPORT[@],@PACKAGE_BUGREPORT@,g' \
		-e 's,[@]GIT_PACKAGE_VERSION[@],@GIT_PACKAGE_VERSION@,g' \
		-e 's,[@]INSTALL[@],$(INSTALL),g' \
		-e 's,[@]INSTALL_PROGRAM[@],$(INSTALL_PROGRAM),g' \
		-e 's,[@]INSTALL_SCRIPT[@],$(INSTALL_SCRIPT),g' \
		-e 's,[@]INSTALL_DATA[@],$(INSTALL_DATA),g' \
		-e 's,[@]prefix[@],$(prefix),g' \
		-e 's,[@]datadir[@],$(datadir),g' \
		-e 's,[@]sysconfdir[@],$(sysconfdir),g'
do_subst_fn = \
		echo "do_subst < ${@}.in > ${@}"; \
		mkdir -p `dirname ${@}` || exit 1; \
		${do_subst} < $(srcdir)/${@}.in > ${@} || exit 1; \
		chmod 0755 ${@}
do_subst_dt = \
		echo "do_subst < ${@}.in > ${@}"; \
		mkdir -p `dirname ${@}` || exit 1; \
		${do_subst} < $(srcdir)/${@}.in > ${@} || exit 1; \
		chmod 0644 ${@}


# custom targets
.PHONY: data

data: data/infected.tar.gz data/non-infected.tar.gz

data/infected.tar.gz: Makefile data/invert-file $(srcdir)/data/infected.tar.gz.bin
	mkdir -p data
	./data/invert-file $(srcdir)/data/infected.tar.gz.bin \
		data/infected.tar.gz

data/non-infected.tar.gz: Makefile data/invert-file $(srcdir)/data/non-infected.tar.gz.bin
	mkdir -p data
	./data/invert-file $(srcdir)/data/non-infected.tar.gz.bin \
		data/non-infected.tar.gz


# local targets
clean-local:
	rm -f data/infected.tar.gz
	rm -f data/non-infected.tar.gz

distclean-local:
	-rm -fR $(srcdir)/autom4te.cache	# removes autoconf cache

install-exec-local:

install-data-local:

install-exec-hook:

install-data-hook:

uninstall-local:

uninstall-hook:


# end of autmake script
