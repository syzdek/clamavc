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
 *  @file tests/multiscan.c tests clamavc_multiscan() function
 */
#define _CLAMAVC_TESTS_CONTSCAN_C 1
#include "common.h"

/////////////////
//             //
//  Functions  //
//             //
/////////////////

/// main statement
int main (void)
{
   CLAMAVC      * clamp;

   if (config(&clamp))
      return(1);
   if (!(clamp))
      return(1);

   if (clamd_remote)
   {
      clamavc_close(clamp);
      return(77);
   };

   if (clamavc_multiscan(clamp, infected_dir) != 1)
   {
      clamavc_close(clamp);
      return(1);
   };

   if (clamavc_multiscan(clamp, clean_archive) != 0)
   {
      clamavc_close(clamp);
      return(1);
   };

   if (clamavc_multiscan(clamp, infected_archive) != 1)
   {
      clamavc_close(clamp);
      return(1);
   };

   if (clamavc_multiscan(clamp, clean_dir) != 0)
   {
      clamavc_close(clamp);
      return(1);
   };

   if (clamavc_multiscan(clamp, infected_file) != 1)
   {
      clamavc_close(clamp);
      return(1);
   };

   if (clamavc_multiscan(clamp, clean_file) != 0)
   {
      clamavc_close(clamp);
      return(1);
   };

   clamavc_close(clamp);

   return(0);
}

/* end of source code */
