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
 *  @file tests/common.c common functions for tests
 */
#define _CLAMAVC_TESTS_COMMON_C 1
#include "common.h"

///////////////
//           //
//  Headers  //
//           //
///////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


////////////////////////
//                    //
//  Global Variables  //
//                    //
////////////////////////

int clamd_remote = 0;
int clamd_socket = 0;

char clean_archive[1024];
char clean_dir[1024];
char clean_file[1024];

char infected_archive[1024];
char infected_dir[1024];
char infected_file[1024];


/////////////////
//             //
//  Functions  //
//             //
/////////////////

/// processed command line options
/// @param[in]  argc     number of arguments passed to clamavclient
/// @param[in]  argv     array of arguments passed to clamavclient
/// @param[out] clampp   pointer to ClamAV Client session data
/// @param[out] filep    pointer to file pointer
int config(CLAMAVC ** clampp)
{
   CLAMAVC      * clamp;
   uintmax_t      uval;
   const char   * str;

   clamd_remote = 1;
   clamd_socket = 0;
   *clampp      = NULL;

   if (!(str = getenv("ABS_TOP_SRCDIR")))
      return(1);
   if (1023 <= snprintf(clean_dir, 1024,     "%s/data", str)) return(1);

   if (!(str = getenv("ABS_TOP_BUILDDIR")))
      return(1);
   if (1023 <= snprintf(infected_dir, 1024,     "%s/data", str)) return(1);

   if (1023 <= snprintf(clean_file, 1024,       "%s/clean.txt",           infected_dir)) return(1);
   if (1023 <= snprintf(clean_archive, 1024,    "%s/clean.tar.gz",        infected_dir)) return(1);
   if (1023 <= snprintf(infected_file, 1024,    "%s/infected.txt",        infected_dir)) return(1);
   if (1023 <= snprintf(infected_archive, 1024, "%s/infected.tar.gz",     infected_dir)) return(1);   

   if (!(clamp = clamavc_initialize()))
      return(-1);

   uval = 0;;
   clamavc_set_opt(clamp, CLAMAVC_OVERBOSE, &uval);

   str = getenv("CLAMAVC_HOST") ? getenv("CLAMAVC_HOST") : CLAMAVC_HOST;
   if (str[0] == '/')
   {
      clamd_remote = 1;
      clamd_socket = 1;
   }
   else if ( (!(strcmp(str, "localhost"))) ||
             (!(strcmp(str, "127.0.0.1"))) ||
             (!(strcmp(str, "::1"))) )
   {
      clamd_remote = 0;
      clamd_socket = 0;
   }
   else
   {
      clamd_remote = 1;
      clamd_socket = 0;
   };

   if ((str = getenv("CLAMAVC_REMOTE")))
   {
      clamd_remote = 1;
      clamd_socket = 0;
   };

   *clampp = clamp;

   return(0);
}


/* end of source code */
