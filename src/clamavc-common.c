/*
 *  Clam Antivirus Client Library
 *  Copyright (C) 2009 Alaska Communications Systems Group, Inc
 *
 *  @ACS_LICENSE_HEADER_START@
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  @ACS_LICENSE_HEADER_END@
 */
/**
 *  @file examples/clamavc-common.c common functions for examples
 */
#define _CLAMAVC_SRC_CLAMAVC_COMMON_C 1
#include "clamavc-common.h"

/////////////////
//             //
//  Functions  //
//             //
/////////////////

/// prints program usage and exits
void usage(void)
{
   // TRANSLATORS: The following strings provide usage for command. These
   // strings are displayed if the program is passed `--help' on the command
   // line. The two strings referenced are: PROGRAM_NAME, and
   // PACKAGE_BUGREPORT
   printf(_("Usage: %s [options] file\n"
         "  -h host                   host name of ClamAV server\n"
         "  -H, --help                print this help and exit\n"
         "  -p port                   TCP port number of ClamAV server\n"
         "  -s bytes                  max stream chunk size to send\n"
         "  -v, --verbose             print verbose messages\n"
         "  -V, --version             print version number and exit\n"
         "\nReport bugs to <%s>.\n"
      ), PROGRAM_NAME, PACKAGE_BUGREPORT
   );
   return;
}


/// prints program version and exits
void version(void)
{
   // TRANSLATORS: The following strings provide version and copyright
   // information if the program is passed --version on the command line.
   // The three strings referenced are: PROGRAM_NAME, PACKAGE_NAME,
   // PACKAGE_VERSION.
   printf(_( "%s (%s) %s\n"
         "Written by Alaska Communications Systems Group, Inc.\n"
         "\n"
         "Copyright (C) 2009 Alaska Communications Systems Group, Inc.\n"
         "This is proprietary software; copying the source code or binary is prohibited.\n"
         "There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR\n"
         "PURPOSE.\n"
      ), PROGRAM_NAME, PACKAGE_NAME, PACKAGE_VERSION
   );
   return;
}


/* end of source code */
