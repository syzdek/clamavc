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
 *  @file lib/libclamavc.c libclamav.la source code
 */
#define _CLAMAVC_LIB_LIBCLAMAVC_C 1

///////////////
//           //
//  Headers  //
//           //
///////////////

#include <clamavc.h>
#include <stdlib.h>
#include <string.h>


/////////////////
//             //
//  Datatypes  //
//             //
/////////////////

/// internal libclamavc.la state data
struct clamavc
{
   int    port;   ///< TCP port accepting connections for the ClamAV daemon
   char * host;   ///< network host running the ClamAV daemon
   char * socket; ///< unix domain socket for connections to the ClamAV daemon
};


/////////////////
//             //
//  Functions  //
//             //
/////////////////

/// closes ClamAV client library session and frees resources
/// @param[in]  clamp    pointer to ClamAV Client session data
/// @param[in]  argv     array of arguments passed to clamavclient
void clamavc_close(CLAMAVC * clamp)
{
   if (!(clamp))
      return;

   memset(clamp, 0, sizeof(CLAMAVC));
   free(clamp);

   return;
}


/// initialize ClamAV client library session
CLAMAVC * clamavc_initialize(void)
{
   CLAMAVC * clamp;

   if (!(clamp = malloc(sizeof(CLAMAVC))))
      return(NULL);
   memset(clamp, 0, sizeof(CLAMAVC));

   return(clamp);
}


/* end of source code */
