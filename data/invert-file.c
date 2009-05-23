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
 *  @file data/invert-file.c inverts the bits of a file
 */
#define _CLAMAVC_DATA_INVERT_FILE_C 1

///////////////
//           //
//  Headers  //
//           //
///////////////

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////

// main statement
int main (int argc, char * argv[]);


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
   int    i;
   int    id;
   int    od;
   int    len;
   char * infile;
   char * outfile;
   char   buff[4096];

   if (argc != 3)
   {
      fprintf(stderr, "Usage: %s <in> <out>\n", argv[0]);
      return(1);
   };

   infile  = argv[1];
   outfile = argv[2];

   if ((id = open(infile, O_RDONLY)) == -1)
   {
      perror("open()");
      return(1);
   };

   if ((od = open(outfile, O_WRONLY|O_CREAT)) == -1)
   {
      perror("open()");
      close(id);
      return(1);
   };

   if ((fchmod(od, 0644)) == -1)
   {
      perror("fchmod()");
      close(id);
      close(od);
   };

   while((len = read(id, buff, 1024)) > 0)
   {
      for(i = 0; i < len; i++)
         buff[i] = buff[i] ^ 0xFF;
      write(od, buff, len);
   };
   if (len == -1)
   {
      perror("read()");
      close(id);
      close(od);
      return(1);
   };

   close(id);
   close(od);

   return(0);
}

/* end of source code */
