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
 *  @file tests/idsession-all.c tests all idsession functions in serial
 */
#define _CLAMAVC_TESTS_IDSESSION_ALL_C 1
#include "common.h"

/////////////////
//             //
//  Functions  //
//             //
/////////////////

/// main statement
int main (void)
{
   int            i;
   char         * data;
   int32_t        uval;
   CLAMAVC      * clamp;
   const char   * ver;

   if (config(&clamp))
      return(1);
   if (!(clamp))
      return(1);

   if (clamd_remote)
   {
      clamavc_close(clamp);
      return(77);
   };

   for(i = 0; i < 3; i++)
   {
      if (!(ver = clamavc_version(clamp)))
         { clamavc_close(clamp); return(1); };

      uval = 8;
      clamavc_set_opt(clamp, CLAMAVC_OSTREAMMAXLEN, &uval);

      if (instream_file(clamp, clean_archive) != 0)
         { clamavc_close(clamp); return(1); };

      if (instream_file(clamp, infected_archive) != 1)
         { clamavc_close(clamp); return(1); };

      if (instream_file(clamp, infected_file) != 1)
         { clamavc_close(clamp); return(1); };

      if (instream_file(clamp, clean_file) != 0)
         { clamavc_close(clamp); return(1); };

      if (instream_fildes(clamp, clean_archive) != 0)
         { clamavc_close(clamp); return(1); };

      if (instream_fildes(clamp, infected_archive) != 1)
         { clamavc_close(clamp); return(1); };

      if (instream_fildes(clamp, infected_file) != 1)
         { clamavc_close(clamp); return(1); };

      if (instream_fildes(clamp, clean_file) != 0)
         { clamavc_close(clamp); return(1); };

      uval = 4096;
      data = getenv("CLAMAVC_STREAMMAXLEN");
      uval = data ? strtoul(data, NULL, 0)  : CLAMAVC_STREAMMAXLEN;
      clamavc_set_opt(clamp, CLAMAVC_OSTREAMMAXLEN, &uval);

      if (instream_file(clamp, clean_archive) != 0)
         { clamavc_close(clamp); return(1); };

      if (instream_file(clamp, infected_archive) != 1)
         { clamavc_close(clamp); return(1); };

      if (instream_file(clamp, infected_file) != 1)
         { clamavc_close(clamp); return(1); };

      if (instream_file(clamp, clean_file) != 0)
         { clamavc_close(clamp); return(1); };

      if (instream_fildes(clamp, clean_archive) != 0)
         { clamavc_close(clamp); return(1); };

      if (instream_fildes(clamp, infected_archive) != 1)
         { clamavc_close(clamp); return(1); };

      if (instream_fildes(clamp, infected_file) != 1)
         { clamavc_close(clamp); return(1); };

      if (instream_fildes(clamp, clean_file) != 0)
         { clamavc_close(clamp); return(1); };

      if (clamavc_scan(clamp, infected_dir) != 1)
         { clamavc_close(clamp); return(1); };

      if (clamavc_scan(clamp, clean_archive) != 0)
         { clamavc_close(clamp); return(1); };

      if (clamavc_scan(clamp, infected_archive) != 1)
         { clamavc_close(clamp); return(1); };

      if (clamavc_scan(clamp, clean_dir) != 0)
         { clamavc_close(clamp); return(1); };

      if (clamavc_scan(clamp, infected_file) != 1)
         { clamavc_close(clamp); return(1); };

      if (clamavc_scan(clamp, clean_file) != 0)
         { clamavc_close(clamp); return(1); };
   };

   clamavc_close(clamp);

   return(0);
}

/* end of source code */
