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

all: invert-file infected.tar.gz non-infected.tar.gz

invert-file: invert-file.c

infected.tar.gz: invert-file infected.tar.gz.bin
	./invert-file infected.tar.gz.bin infected.tar.gz

non-infected.tar.gz: invert-file non-infected.tar.gz.bin
	./invert-file non-infected.tar.gz.bin non-infected.tar.gz

clean:
	rm -f invert-file
	rm -f infected.tar.gz
	rm -f non-infected.tar.gz

# end of Makefile
