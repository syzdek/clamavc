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
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////

#define CLAMAVC_BUFF_LEN   128
#define CLAMAVC_VER_LEN    128


/////////////////
//             //
//  Datatypes  //
//             //
/////////////////

/// internal libclamavc.la state data
struct clamavc
{
   int         s;            ///< socket handle to ClamAV daemon
   int         port;         ///< TCP port accepting connections for the ClamAV daemon
   int         streamMaxLen; ///< max chunck size to send to clamd 
   int         verbose;      ///< toggle verbose mode
   char      * host;         ///< network host running the ClamAV daemon
   char      * socket;       ///< unix domain socket for connections to the ClamAV daemon
   char        version[CLAMAVC_VER_LEN];
};

//////////////////
//              //
//  Prototypes  //
//              //
//////////////////

// connects to remote ClamAV daemon
int32_t clamavc_connect PARAMS((CLAMAVC * clamp));

// disconnects from remote ClamAV daemon
void clamavc_disconnect PARAMS((CLAMAVC * clamp));


/////////////////
//             //
//  Functions  //
//             //
/////////////////

/// closes ClamAV client library session and frees resources
/// @param[in]  clamp    pointer to ClamAV Client session data
void clamavc_close(CLAMAVC * clamp)
{
   if (!(clamp))
      return;

   if (clamp->host)
      free(clamp->host);
   if (clamp->socket)
      free(clamp->socket);

   memset(clamp, 0, sizeof(CLAMAVC));
   free(clamp);

   return;
}


/// connects to remote ClamAV daemon
/// @param[in]  clamp    pointer to ClamAV Client session data
int32_t clamavc_connect(CLAMAVC * clamp)
{
   int                   i;
   int                   s;
   int                   opt;
   int                   port;
   char                  addr[INET6_ADDRSTRLEN+1];
   struct hostent      * hp;
   //struct sockaddr     * sa;
   struct sockaddr_in    sa;
   struct sockaddr_in6   sa6;

   if (!(clamp))
      return(errno = EINVAL);

   if (clamp->s != -1)
      return(0);

   port = ntohs(clamp->port & 0xFFFF);

   // attempts IPv6 connection
   if ((hp = gethostbyname2(clamp->host, AF_INET6)))
      for(i = 0; hp->h_addr_list[i]; i++)
      {
         memset(&sa6, 0, sizeof(struct sockaddr_in6));
         memcpy(&sa6.sin6_addr, hp->h_addr_list[i], hp->h_length);
         sa6.sin6_family = hp->h_addrtype;
         sa6.sin6_port   = port;
         sa6.sin6_len    = sizeof(struct sockaddr_in6);

         if ((s = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
            return(errno);

         opt = 1;
         setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(int));
         setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (void *)&opt, sizeof(int));

         if (clamp->verbose)
         {
            inet_ntop(sa6.sin6_family, &sa6.sin6_addr, addr, INET6_ADDRSTRLEN);
            addr[INET6_ADDRSTRLEN] = '\0';
            printf("Trying %s...\n", addr);
         };

         if (connect(s, (struct sockaddr *)&sa6, sizeof(sa6)) != -1)
         {
            if (clamp->verbose)
               printf("Connected to %s.\n", clamp->host);
            //fcntl(s, F_SETFL, O_NONBLOCK);
            clamp->s = s;
            return(0);
         };

         if (clamp->verbose)
            printf("connect to address %s: %s\n", addr, strerror(errno));

         close(s);
      };

   // attempts IPv4 connection
   if ((hp = gethostbyname2(clamp->host, AF_INET)))
      for(i = 0; hp->h_addr_list[i]; i++)
      {
         memset(&sa, 0, sizeof(struct sockaddr_in));
         memcpy(&sa.sin_addr, hp->h_addr_list[i], hp->h_length);
         sa.sin_family = hp->h_addrtype;
         sa.sin_port   = port;
         sa.sin_len    = sizeof(struct sockaddr_in6);

         if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            return(errno);

         opt = 1;
         setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(int));
         setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (void *)&opt, sizeof(int));

         if (clamp->verbose)
         {
            inet_ntop(sa.sin_family, &sa.sin_addr, addr, INET6_ADDRSTRLEN);
            addr[INET6_ADDRSTRLEN] = '\0';
            printf("Trying %s...\n", addr);
         };

         if (connect(s, (struct sockaddr *)&sa, sizeof(sa)) != -1)
         {
            if (clamp->verbose)
               printf("Connected to %s.\n", clamp->host);
            //fcntl(s, F_SETFL, O_NONBLOCK);
            clamp->s = s;
            return(0);
         };

         if (clamp->verbose)
            printf("connect to address %s: %s\n", addr, strerror(errno));

         close(s);
      };

   return(errno);
}


/// disconnects from remote ClamAV daemon
/// @param[in]  clamp    pointer to ClamAV Client session data
void clamavc_disconnect(CLAMAVC * clamp)
{
   if (!(clamp))
      return;
   if (clamp->s == -1)
      return;
   close(clamp->s);
   clamp->s = -1;
   return;
}


/// initialize ClamAV client library session
CLAMAVC * clamavc_initialize(void)
{
   CLAMAVC * clamp;

   if (!(clamp = malloc(sizeof(CLAMAVC))))
   {
      errno = ENOMEM;
      return(NULL);
   };
   memset(clamp, 0, sizeof(CLAMAVC));

   // applies defaults
   clamp->s            = -1;
   clamp->port         = CLAMAVC_PORT;
   clamp->streamMaxLen = CLAMAVC_STREAMMAXLEN;
   if (!(clamp->host = strdup(CLAMAVC_HOST)))
   {
      errno = ENOMEM;
      clamavc_close(clamp);
      return(NULL);
   };

   return(clamp);
}


/// checks the daemon's state
/// @param[in]  clamp    pointer to ClamAV Client session data
int32_t clamavc_ping(CLAMAVC * clamp)
{
   char    buff[1024];
   ssize_t len;

   if (clamavc_connect(clamp))
      return(errno);

   printf(">>> zPING\n");
   if ((len = write(clamp->s, "zPING", 6)) == -1)
      return(errno);

   if ((len = read(clamp->s, buff, 1023)) == -1)
      return(errno);

   buff[len] = '\0';
   printf("<<< %s\n", buff);

   return(0);
}


/// sets library options
/// @param[in]  clamp    pointer to ClamAV Client session data
/// @param[in]  opt      numeric ID of option to set
/// @param[in]  valp     pointer to new value of option
int32_t clamavc_set_opt(CLAMAVC * clamp, uint32_t opt, const void * valp)
{
   if ( (!(clamp)) || (!(opt)) )
      return(errno = EINVAL);

   switch(opt)
   {
      case CLAMAVC_OHOST:
         if (clamp->socket)
            free(clamp->socket);
         clamp->socket = NULL;
         if (clamp->host)
            free(clamp->host);
         clamp->host = NULL;
         if (valp)
            if (!(clamp->host = strdup((const char *)valp)))
               return(errno = ENOMEM);
         break;

      case CLAMAVC_OPORT:
         clamp->port = CLAMAVC_PORT;
         if (valp)
            clamp->port = *((const int *)valp) & 0xFFFF;
         break;

      case CLAMAVC_OSOCKET:
         if (clamp->socket)
            free(clamp->socket);
         clamp->socket = NULL;
         if (clamp->host)
            free(clamp->host);
         clamp->host = NULL;
         if (valp)
            if (!(clamp->socket = strdup((const char *)valp)))
               return(errno = ENOMEM);
         break;

      case CLAMAVC_OSTREAMMAXLEN:
         clamp->port = CLAMAVC_STREAMMAXLEN;
         if (valp)
            clamp->streamMaxLen = *((const int *)valp);
         break;

      case CLAMAVC_OVERBOSE:
         clamp->verbose = 0;
         if (valp)
            clamp->verbose = *((const int *)valp);
         break;

      default:
         return(errno = EINVAL);
   };

   return(0);
}


/* end of source code */