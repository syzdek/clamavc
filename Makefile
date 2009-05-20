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
#   Makefile - automate compiling on a unix platform
#

CC	?=  gcc
LIBTOOL	?= libtool
CFLAGS	?= -g -O2 -W -Wall -Werror -Iinclude
LDFLAGS	?= -g -O2 -export-symbols lib/libclamavc.sym -rpath /usr/local/lib -Llib

all: clamavclient libclamavc.la

libclamavc.la: Makefile lib/libclamavc.c include/clamavc.h lib/libclamavc.sym
	$(LIBTOOL) --mode=compile --tag=CC $(CC) $(CFLAGS)  -c lib/libclamavc.c
	$(LIBTOOL) --mode=link    --tag=CC $(CC) $(LDFLAGS) -o libclamavc.la libclamavc.lo

clamavclient: Makefile src/clamavclient.c libclamavc.la
	$(LIBTOOL) --mode=compile --tag=CC $(CC) $(CFLAGS)  -c src/clamavclient.c
	$(LIBTOOL) --mode=link    --tag=CC $(CC) $(LDFLAGS) -o clamavclient clamavclient.lo -lclamavc

clean:
	$(LIBTOOL) --mode=clean rm -f clamavclient clamavclient.lo
	$(LIBTOOL) --mode=clean rm -f libclamavc.la libclamavc.lo
	rm -fR .libs

install: all

uninstall:

# end of Makefile
