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

/// processed command line options
/// @param[in]  argc     number of arguments passed to clamavclient
/// @param[in]  argv     array of arguments passed to clamavclient
/// @param[out] clampp   pointer to ClamAV Client session data
/// @param[out] filep    pointer to file pointer
int config(int argc, char * argv[], CLAMAVC ** clampp, const char ** filep)
{
   CLAMAVC      * clamp;
   int32_t        c;
   int32_t        option_index;
   int32_t        verbose;
   uintmax_t      uval;
   const char   * str;

   static char   short_options[] =  "h:Hp:s:vV";
   static struct option long_options[] =
   {
      {"help",          no_argument, 0, 'H'},
      {"verbose",       no_argument, 0, 'v'},
      {"version",       no_argument, 0, 'V'},
      {NULL,            0,           0, 0  }
   };

   option_index = 0;
   verbose      = 0;
   *filep       = NULL;
   *clampp      = NULL;

   if (!(clamp = clamavc_initialize()))
   {
      perror(PROGRAM_NAME ": clamavc_initialize()");
      return(-1);
   };

   while((c = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1)
   {
      switch(c)
      {
         case -1:    // no more arguments
         case 0:     // long options toggles
            break;

         case 'h':
            if (clamavc_set_opt(clamp, CLAMAVC_OHOST, optarg))
            {
               perror(PROGRAM_NAME ": clamavc_set_opt()");
               clamavc_close(clamp);
               return(-1);
            };
            break;

         case 'H':
            usage();
            clamavc_close(clamp);
            return(0);

         case 'p':
            uval = strtoumax(optarg, NULL, 0);
            clamavc_set_opt(clamp, CLAMAVC_OPORT, &uval);
            break;

         case 's':
            uval = strtoumax(optarg, NULL, 0);
            clamavc_set_opt(clamp, CLAMAVC_STREAMMAXLEN, &uval);
            break;

         case 'v':
            verbose++;
            clamavc_set_opt(clamp, CLAMAVC_OVERBOSE, &verbose);
            break;

         case 'V':
            version();
            clamavc_close(clamp);
            return(0);

         case '?':   // argument error
            fprintf(stderr, _("Try `%s --help' for more information.\n"), PROGRAM_NAME);
            return(-1);

         default:
            fprintf(stderr, _("%s: unrecognized option `--%c'\n"), PROGRAM_NAME, c);
            fprintf(stderr, _("Try `%s --help' for more information.\n"), PROGRAM_NAME);
            return(-1);
      };
   };

   if (filep)
   {
      if ((optind+1) != argc)
      {
         fprintf(stderr, _("%s: missing required argument\n"), PROGRAM_NAME);
         fprintf(stderr, _("Try `%s --help' for more information.\n"), PROGRAM_NAME);
         clamavc_close(clamp);
         return(-1);
      };
   } else {
      if ((optind) != argc)
      {
         fprintf(stderr, _("%s: unknown argument -- %s\n"), PROGRAM_NAME, argv[optind-1]);
         fprintf(stderr, _("Try `%s --help' for more information.\n"), PROGRAM_NAME);
         clamavc_close(clamp);
         return(-1);
      };
   };

   if (verbose)
   {
      if (!(str = clamavc_version(clamp)))
      {
         perror("clamavc_version()");
         clamavc_close(clamp);
         return(1);
      };
      printf("%s\n", str);
   };

   if (filep)
      *filep  = argv[optind];
   *clampp = clamp;

   return(0);
}


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
