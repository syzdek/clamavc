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
 *  @file tests/instream-chunked.c tests clamavc_instream() function
 */
#define _CLAMAVC_TESTS_INSTREAM_C 1
#include "common.h"

///////////////
//           //
//  Headers  //
//           //
///////////////

#include <fcntl.h>


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////

// sends file as chuncked data to the server
int32_t instream_file PARAMS((CLAMAVC * clamp, const char * file));


/////////////////
//             //
//  Functions  //
//             //
/////////////////

/// sends file as chuncked data to the server
/// @param[in]  clamp    pointer to ClamAV Client session data
/// @param[in]  file     file to scan
int32_t instream_file(CLAMAVC * clamp, const char * file)
{
   int            fd;
   int            len;
   int32_t        err;
   char           buff[1024];

   if ((fd = open(file, O_RDONLY)) == -1)
      return(-1);

   while((len = read(fd, buff, 1024)) > 0)
   {
      if ((err = clamavc_instream(clamp, buff, (unsigned)len)) == -1)
      {
         close(fd);
         return(-1);
      };
   };

   close(fd);

   if (len == -1)
      return(-1);

   return(clamavc_instream(clamp, NULL, 0));
}


/// main statement
int main (void)
{
   int32_t        uval;
   CLAMAVC      * clamp;

   if (config(&clamp))
      return(1);
   if (!(clamp))
      return(1);

   uval = 8;
   clamavc_set_opt(clamp, CLAMAVC_OSTREAMMAXLEN, &uval);

   if (instream_file(clamp, clean_archive) != 0)
   {
      clamavc_close(clamp);
      return(1);
   };

   if (instream_file(clamp, infected_archive) != 1)
   {
      clamavc_close(clamp);
      return(1);
   };

   if (instream_file(clamp, infected_file) != 1)
   {
      clamavc_close(clamp);
      return(1);
   };

   if (instream_file(clamp, clean_file) != 0)
   {
      clamavc_close(clamp);
      return(1);
   };

   clamavc_close(clamp);

   return(0);
}

/* end of source code */
