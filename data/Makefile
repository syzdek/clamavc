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

all: clean.tar.gz clean.txt infected.tar.gz infected.txt invert-file

clean.tar.gz: invert-file clean.tar.gz.bin
	./invert-file clean.tar.gz.bin clean.tar.gz

clean.txt: invert-file clean.txt.bin
	./invert-file clean.txt.bin clean.txt

infected.tar.gz: invert-file infected.tar.gz.bin
	./invert-file infected.tar.gz.bin infected.tar.gz

infected.txt: invert-file infected.txt.bin
	./invert-file infected.txt.bin infected.txt

invert-file: invert-file.c

clean:
	rm -f infected.tar.gz
	rm -f infected.txt
	rm -f invert-file *.o
	rm -f clean.tar.gz
	rm -f clean.txt

# end of Makefile
