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
 *  @file tests/common.h common functions for tests
 */
#ifndef _CLAMAVC_TESTS_COMMON_H
#define _CLAMAVC_TESTS_COMMON_H 1


///////////////
//           //
//  Headers  //
//           //
///////////////

#include <clamavc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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


////////////////////////
//                    //
//  Global Variables  //
//                    //
////////////////////////

extern int clamd_remote;
extern int clamd_socket;

extern char clean_archive[1024];
extern char clean_dir[1024];
extern char clean_file[1024];

extern char infected_archive[1024];
extern char infected_dir[1024];
extern char infected_file[1024];


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////

// processed command line options
int config PARAMS((CLAMAVC ** clampp));

// main statement
int main PARAMS((void));


#endif /* end of source code */
