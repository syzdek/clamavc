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
 *  @file examples/clamavc-common.h common functions for examples
 */
#define _CLAMAVC_SRC_CLAMAVC_COMMON_H 1


///////////////
//           //
//  Headers  //
//           //
///////////////

#include <clamavc.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////

#ifndef PACKAGE_BUGREPORT
#define PACKAGE_BUGREPORT "opensource@acsalaska.net"
#endif

#ifndef PACKAGE_NAME
#define PACKAGE_NAME ""
#endif

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION ""
#endif

#ifndef PROGRAM_NAME
#define PROGRAM_NAME "clamavc"
#endif


///////////////////
//               //
//  i18l Support //
//               //
///////////////////

#ifdef HAVE_GETTEXT
#   include <gettext.h>
#   include <libintl.h>
#   define _(String) gettext (String)
#   define gettext_noop(String) String
#   define N_(String) gettext_noop (String)
#else
#   define _(String) (String)
#   define N_(String) String
#   define textdomain(Domain)
#   define bindtextdomain(Package, Directory)
#endif


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////

// processed command line options
int config PARAMS((int argc, char * argv[], CLAMAVC ** clampp,
   const char ** filep));

// prints program usage and exits
void usage PARAMS((void));

// prints program version and exits
void version PARAMS((void));


/* end of source code */
