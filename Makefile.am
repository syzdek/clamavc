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
noinst_PROGRAMS				= src/clamavc-instream
include_HEADERS				= include/clamavc.h
lib_LTLIBRARIES				= lib/libclamavc.la
man_MANS				=


# macros for lib/libclamavc.la
lib_libclamavc_la_DEPENDENCIES		= Makefile lib/libclamavc.sym
lib_libclamavc_la_CFLAGS		= $(AM_CFLAGS)
lib_libclamavc_la_LIBADD		= $(AM_LIBS)
lib_libclamavc_la_LDFLAGS		= -rpath '$(libdir)' \
					  -version-info $(LIB_VERSION_INFO) \
					  $(AM_LDFLAGS) \
					  -export-symbols $(srcdir)/lib/libclamavc.sym
lib_libclamavc_la_SOURCES		= include/clamavc.h lib/libclamavc.c


# macros for src/clamavc
src_clamavc_instream_DEPENDENCIES	= Makefile lib/libclamavc.la
src_clamavc_instream_CPPFLAGS		= -DPROGRAM_NAME="\"clamavc-instream\"" $(AM_CPPFLAGS)
src_clamavc_instream_CFLAGS		= $(AM_CFLAGS)
src_clamavc_instream_LDFLAGS		= $(AM_LDFLAGS)
src_clamavc_instream_LDADD		= $(AM_LDADD) lib/libclamavc.la
src_clamavc_instream_SOURCES		= include/clamavc.h \
					  src/clamavc-common.c \
					  src/clamavc-common.h \
					  src/clamavc-instream.c


# miscellaneous lists
CLEANFILES				= $(builddir)/a.out   $(srcdir)/a.out \
					  $(builddir)/*/a.out $(srcdir)/*/a.out \
					  $(builddir)/doc/otadm.conf-example \
					  config.h.in~ $(srcdir)/config.h.in~ \
					  stamp-vti $(srcdir)/stamp-vti \
					  $(man_MANS) \
					  @PACKAGE_TARNAME@-*.tar.* \
					  @PACKAGE_TARNAME@-*.zip
EXTRA_DIST				= lib/libclamavc.sym
EXTRA_PROGRAMS				=
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


# local targets
clean-local:

distclean-local:
	-rm -fR $(srcdir)/autom4te.cache	# removes autoconf cache

install-exec-local:

install-data-local:

install-exec-hook:

install-data-hook:

uninstall-local:

uninstall-hook:


# end of autmake script
