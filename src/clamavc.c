/*
 *  Clam Antivirus Client Library
 *  Copyright (C) 2009 David M. Syzdek <david@syzdek.net>
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
 *  @file src/clamavc.c simple client using libclamavc.la
 */
#define _CLAMAVC_SRC_CLAMAVC_C 1

///////////////
//           //
//  Headers  //
//           //
///////////////

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <clamavc.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
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


/////////////////
//             //
//  Datatypes  //
//             //
/////////////////

/// internal clamavc config data
typedef struct clamavc_config MyConfig;

/// stack of directory listings
typedef struct clamavc_stack MyStack;

/// internal clamavc config data
struct clamavc_config
{
   int       cont;
   int       hidden;
   int       quiet;
   int       recursive;
   int       verbose;
   CLAMAVC  * clamp;
   MyStack  * stack;
   MyStack  * queue;
};

/// stack of directory listings
struct clamavc_stack
{
   char    * path;
   MyStack * previous;
   MyStack * next;
};


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

// main statement
int main PARAMS((int argc, char * argv[]));

// processed command line options
int my_config PARAMS((int argc, char * argv[], MyConfig * cnfp));

// scans files/directories in the stack
int my_scan PARAMS((MyConfig * cnf));

// scans STDIN in the stack
int my_scan_stdin PARAMS((CLAMAVC * clamp, int fildes));

// dequeues data from the stack
char * my_stack_dequeue PARAMS((MyConfig * cnfp));

// pops data from the stack
char * my_stack_pop PARAMS((MyConfig * cnf));

// pushes data to the stack
int my_stack_push PARAMS((MyConfig * cnf, const char * path1,
   const char * path2));

// prints program usage and exits
void my_usage PARAMS((void));

// prints program version and exits
void my_version PARAMS((void));


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
   int        err;
   char     * ptr;
   MyConfig   cnf;

#ifdef HAVE_GETTEXT
   setlocale (LC_ALL, "");
   bindtextdomain (PACKAGE, LOCALEDIR);
   textdomain (PACKAGE);
#endif

   if ((err = my_config(argc, argv, &cnf)))
      return(err);
   if (!(cnf.clamp))
      return(0);

   if ((err = my_scan(&cnf)))
   {
      while((ptr = my_stack_pop(&cnf)))
         free(ptr);
      return(err);
   };

   clamavc_close(cnf.clamp);

   return(0);
}


/// processed command line options
/// @param[in]  argc     number of arguments passed to clamavclient
/// @param[in]  argv     array of arguments passed to clamavclient
/// @param[out] cnfp     pointer to config data
int my_config(int argc, char * argv[], MyConfig * cnfp)
{
   int          opt_index;
   int          c;
   int          i;
   int          err;
   unsigned     uval;
   const char * str;

   static char   short_opt[] =  "ach:Hp:qrs:vV";
   static struct option long_opt[] =
   {
      {"continue",      no_argument, 0, 'c'},
      {"help",          no_argument, 0, 'H'},
      {"silent",        no_argument, 0, 'q'},
      {"quiet",         no_argument, 0, 'q'},
      {"test",          no_argument, 0, 't'},
      {"verbose",       no_argument, 0, 'v'},
      {"version",       no_argument, 0, 'V'},
      {NULL,            0,           0, 0  }
   };

   memset(cnfp, 0, sizeof(MyConfig));
   cnfp->verbose      = 0;
   opt_index          = 0;

   if (!(cnfp->clamp = clamavc_initialize()))
   {
      perror(PROGRAM_NAME ": clamavc_initialize()");
      return(-1);
   };

   while((c = getopt_long(argc, argv, short_opt, long_opt, &opt_index)) != -1)
   {
      switch(c)
      {
         case -1:    // no more arguments
         case 0:     // long options toggles
            break;

         case 'a':
            cnfp->hidden = 1;
            break;

         case 'c':
            cnfp->cont = 1;
            break;

         case 'h':
            if (clamavc_set_opt(cnfp->clamp, CLAMAVC_OHOST, optarg))
            {
               perror(PROGRAM_NAME ": clamavc_set_opt()");
               clamavc_close(cnfp->clamp);
               return(-1);
            };
            break;

         case 'H':
            my_usage();
            clamavc_close(cnfp->clamp);
            cnfp->clamp = NULL;
            return(0);

         case 'p':
            uval = strtoul(optarg, NULL, 0);
            clamavc_set_opt(cnfp->clamp, CLAMAVC_OPORT, &uval);
            break;

         case 'q':
            cnfp->quiet   = 1;
            cnfp->verbose = 0;
            break;

         case 'r':
            cnfp->recursive = 1;
            break;

         case 's':
            uval = strtoul(optarg, NULL, 0);
            clamavc_set_opt(cnfp->clamp, CLAMAVC_OSTREAMMAXLEN, &uval);
            break;

         case 'v':
            cnfp->quiet = 0;
            cnfp->verbose++;
            clamavc_set_opt(cnfp->clamp, CLAMAVC_OVERBOSE, &cnfp->verbose);
            break;

         case 'V':
            my_version();
            clamavc_close(cnfp->clamp);
            cnfp->clamp = NULL;
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

   if (cnfp->verbose)
   {
      if (!(str = clamavc_version(cnfp->clamp)))
      {
         perror("clamavc_version()");
         clamavc_close(cnfp->clamp);
         cnfp->clamp = NULL;
         return(1);
      };
      printf("%s\n", str);
   };

   if (optind == argc)
   {
      switch(err = my_scan_stdin(cnfp->clamp, STDIN_FILENO))
      {
         case -1:
            fprintf(stderr, "STDIN_FILENO: %s\n", strerror(errno));
            err = 1;
            break;
         case 0:
            break;
         default:
            err = 2;
            if (!(cnfp->quiet))
               fprintf(stderr, "%s\n", clamavc_error(cnfp->clamp));
            break;
      };
      clamavc_close(cnfp->clamp);
      cnfp->clamp = NULL;
      return(err);
   };

   for(i = argc; i > optind; i--)
   {
      if ((my_stack_push(cnfp, argv[i-1], NULL)))
      {
         clamavc_close(cnfp->clamp);
         cnfp->clamp = NULL;
         return(-1);
      };
   };

   return(0);
}


/// scans files/directories in the stack
/// @param[out] cnfp     pointer to config data
int my_scan(MyConfig * cnfp)
{
   int             err;
   DIR           * d;
   char          * path;
   struct stat     sb;
   struct dirent * dp;

   err = 0;

   while((path = my_stack_pop(cnfp)))
   {
      // processes STDIN_FILENO
      if (!(strcmp(path, "-")))
      {
         if (cnfp->verbose)
            printf("processing <STDIN>\n");
         switch(err = my_scan_stdin(cnfp->clamp, STDIN_FILENO))
         {
            case -1:
               fprintf(stderr, "%s: %s\n", path, strerror(errno));
               free(path);
               return(1);
            case 0:
               break;
            default:
               err = 2;
               if (!(cnfp->quiet))
                  fprintf(stderr, "%s\n", clamavc_error(cnfp->clamp));
               if (!(cnfp->cont))
               {
                  free(path);
                  return(err);
               };
               break;
         };
         free(path);
         continue;
      };

      // stat file for file type and file size
      if ((stat(path, &sb)) == -1)
      {
         fprintf(stderr, "%s: %s\n", path, strerror(errno));
         free(path);
         return(1);
      };

      // append contents for directory to stack
      if (sb.st_mode & S_IFDIR)
      {
         if (!(d = opendir(path)))
         {
            fprintf(stderr, "%s: %s\n", path, strerror(errno));
            free(path);
            return(1);
         };
         for(dp = readdir(d); dp; dp = readdir(d))
         {
            if (dp->d_name[0] == '.')
            {
               if ( (!(strcmp(dp->d_name, ".."))) &&
                    (!(strcmp(dp->d_name, ".")))  &&
                    (cnfp->hidden) )
               {
                  if (my_stack_push(cnfp, path, dp->d_name))
                  {
                     free(path);
                     closedir(d);
                     return(1);
                  };
               };
            } else if (my_stack_push(cnfp, path, dp->d_name)) {
               free(path);
               closedir(d);
               return(1);
            };
         };
         closedir(d);
      };

      // scan file for viruses
      if (!(sb.st_mode & S_IFDIR))
      {
         if (cnfp->verbose)
            printf("processing %s\n", path);
         switch(clamavc_instream_file(cnfp->clamp, path))
         {
            case -1:
               fprintf(stderr, "%s: %s\n", path, strerror(errno));
               free(path);
               return(1);
            case 0:
               break;
            default:
               err = 2;
               if (!(cnfp->quiet))
                  fprintf(stderr, "%s: %s\n", path, clamavc_error(cnfp->clamp));
               if (!(cnfp->cont))
               {
                  free(path);
                  return(err);
               };
               break;
         };
      };
      free(path);
   };

   return(err);
}


/// scans STDIN in the stack
/// @param[out] cnfp     pointer to config data
int my_scan_stdin(CLAMAVC * clamp, int fildes)
{
   int  len;
   char buff[1024];
   while((len = read(fildes, buff, 1024)) > 0)
      if (clamavc_instream(clamp, buff, (unsigned)len))
         return(-1);
   if (len == -1)
      return(-1);
   return(clamavc_instream(clamp, NULL, 0));
}


/// pops data from the stack
/// @param[out] cnfp     pointer to config data
char * my_stack_dequeue(MyConfig * cnfp)
{
   char    * path;
   MyStack * queue;
   if (!(cnfp->queue))
      return(NULL);
   queue       = cnfp->queue;
   path        = cnfp->queue->path;
   cnfp->queue = cnfp->queue->next;
   if (cnfp->queue)
      cnfp->queue->previous = NULL;
   else
      cnfp->stack = NULL;
   free(queue);
   return(path);
}


/// pops data from the stack
/// @param[out] cnfp     pointer to config data
char * my_stack_pop(MyConfig * cnfp)
{
   char    * path;
   MyStack * stack;
   if (!(cnfp->stack))
      return(NULL);
   stack       = cnfp->stack;
   path        = cnfp->stack->path;
   cnfp->stack = cnfp->stack->previous;
   if (cnfp->stack)
      cnfp->stack->next = NULL;
   else
      cnfp->queue = NULL;
   free(stack);
   return(path);
}


/// pushes data to the stack
/// @param[out] cnfp     pointer to config data
/// @param[out] path1    path to add to stack
/// @param[out] path2    path to add to stack
int my_stack_push(MyConfig * cnf, const char * path1, const char * path2)
{
   char       * path;
   size_t       len;
   MyStack    * stack;
   const char * delim;

   delim = "/";
   if (!(path1))
      return(0);
   if (!(path2))
   {
      delim = "";
      path2 = "";
   };

   len  = 2;
   len += strlen(path1);
   len += strlen(path2);

   if (!(path = malloc(len)))
   {
      fprintf(stderr, "out of virtual memory\n");
      return(1);
   };

   strncpy(path, path1, len);
   strncat(path, delim, len);
   strncat(path, path2, len);

   if (!(stack = malloc(sizeof(MyStack))))
   {
      fprintf(stderr, "out of virtual memory\n");
      free(stack);
      return(1);
   };

   if (cnf->stack)
      cnf->stack->next = stack;
   stack->path      = path;
   stack->previous  = cnf->stack;
   stack->next      = NULL;
   cnf->stack       = stack;
   if (!(cnf->queue))
      cnf->queue = stack;

   return(0);
}


/// prints program usage and exits
void my_usage(void)
{
   // TRANSLATORS: The following strings provide usage for command. These
   // strings are displayed if the program is passed `-H' on the command
   // line. The two strings referenced are: PROGRAM_NAME, and
   // PACKAGE_BUGREPORT
   printf(_("Usage: %s [options] file\n"
         "  -a                        process hidden files and directories\n"
         "  -c                        continue if virus is found\n"
         "  -h host                   host name of ClamAV daemon\n"
         "  -H, --help                print this help and exit\n"
         "  -q, --quiet, --silent     do not print messages\n"
         "  -p port                   TCP port number of ClamAV server\n"
         "  -r                        recursively scan directories\n"
         "  -s bytes                  max stream chunk size to send\n"
         "  -v, --verbose             print verbose messages\n"
         "  -V, --version             print version number and exit\n"
         "\nReport bugs to <%s>.\n"
      ), PROGRAM_NAME, PACKAGE_BUGREPORT
   );
   return;
}


/// prints program version and exits
void my_version(void)
{
   // TRANSLATORS: The following strings provide version and copyright
   // information if the program is passed --version on the command line.
   // The three strings referenced are: PROGRAM_NAME, PACKAGE_NAME,
   // PACKAGE_VERSION.
   printf(_(
         "%s (%s) %s\n"
         "Written by David M. Syzdek.\n"
         "\n"
         "This is free software; see the source for copying conditions.  There is NO\n"
         "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
      ), PROGRAM_NAME, PACKAGE_NAME, PACKAGE_VERSION
   );
   return;
}


/* end of source code */
