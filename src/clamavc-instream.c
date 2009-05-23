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
 *  @file examples/clamavc-instream.c simple client using libclamavc.la
 */
#define _CLAMAVC_SRC_CLAMAVC_INSTREAM_C 1

///////////////
//           //
//  Headers  //
//           //
///////////////

#include <clamavc.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "clamavc-common.h"


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////

// main statement
int main PARAMS((int argc, char * argv[]));


/////////////////
//             //
//  Functions  //
//             //
/////////////////

/// main statement
/// @param[in]  argc     number of arguments passed to clamavclient
/// @param[in]  argv     array of arguments passed to clamavclient
int main (int argc, char * argv[])
{
   int            fd;
   int            len;
   int32_t        err;
   CLAMAVC      * clamp;
   const char   * file;
   char           buff[4096];

#ifdef HAVE_GETTEXT
   setlocale (LC_ALL, "");
   bindtextdomain (PACKAGE, LOCALEDIR);
   textdomain (PACKAGE);
#endif

   if (config(argc, argv, &clamp, &file))
      return(1);
   if (!(clamp))
      return(0);

   if (clamavc_ping(clamp))
   {
      perror("clamavc_ping()");
      clamavc_close(clamp);
      return(1);
   };

   if ((fd = open(file, O_RDONLY)) == -1)
   {
      perror("open()");
      clamavc_close(clamp);
      return(1);
   };

   while((len = read(fd, buff, 4096)) > 0)
   {
      if ((err = clamavc_instream(clamp, buff, (unsigned)len)) == -1)
      {
         perror("clamavc_instream()");
         close(fd);
         clamavc_close(clamp);
         return(1);
      };
   };
   if (len == -1)
   {
      perror("read()");
      close(fd);
      clamavc_close(clamp);
      return(1);
   };

   switch(err = clamavc_instream(clamp, NULL, 0))
   {
      case -1:
         perror("clamavc_instream()");
         break;
      case 0:
         printf("not infected\n");
         break;
      default:
         printf("infected\n");
         break;
   };

   close(fd);

   clamavc_close(clamp);

   if (err)
      return(1);
   return(0);
}

/* end of source code */
