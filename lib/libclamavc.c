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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef _CLAMAVC_H
#include <clamavc.h>
#endif

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
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
#define CLAMAVC_MSG_LEN    128
#define CLAMAVC_VER_LEN    128

#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN   512
#endif

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

/// internal libclamavc.la state data
struct clamavc
{
   int         s;            ///< socket handle to ClamAV daemon
   unsigned    port;         ///< TCP port accepting connections for the ClamAV daemon
   unsigned    streamMaxLen; ///< max chunck size to send to clamd
   unsigned    verbose;      ///< toggle verbose mode
   unsigned    idsess;       ///< IDSESSION state
   unsigned    instream;     ///< INSTREAM state
   char      * host;         ///< network host running the ClamAV daemon
   char        version[CLAMAVC_VER_LEN];
   char        msg[CLAMAVC_MSG_LEN];
};

//////////////////
//              //
//  Prototypes  //
//              //
//////////////////

// connects to remote ClamAV daemon
int32_t clamavc_connect PARAMS((CLAMAVC * clamp, unsigned idsess));

// converts from host byte order to network byte order
uint32_t clamavc_hton PARAMS((uint32_t host, size_t width));

// reads a data stream from the ClamAV daemon
int32_t clamavc_read PARAMS((CLAMAVC * clamp, char * dst, size_t nbyte));

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

   clamavc_reset(clamp);

   memset(clamp, 0, sizeof(CLAMAVC));
   free(clamp);

   return;
}


/// connects to remote ClamAV daemon
/// @param[in]  clamp    pointer to ClamAV Client session data
int32_t clamavc_connect(CLAMAVC * clamp, unsigned idsess)
{
   int                   i;
   int                   s;
   int                   opt;
   int                   port;
   char                  addr[INET6_ADDRSTRLEN+1];
   struct hostent      * hp;
#ifdef AF_INET
   struct sockaddr_in    sa;
#endif
#ifdef AF_INET6
   struct sockaddr_in6   sa6;
#endif
#ifdef AF_UNIX
   struct sockaddr_un    su;
#endif

   if (!(clamp))
      return(errno = EINVAL);

   if (clamp->instream)
      return(0);

   if (clamp->idsess != idsess)
      clamavc_reset(clamp);

   if (clamp->s != -1)
      return(0);

   port = clamavc_hton(clamp->port, 2);

   // attempts Unix domain socket connection
#ifdef AF_UNIX
   if (clamp->host[0] == '/')
   {
      su.sun_path[103] = '\0';
      strncpy(su.sun_path, clamp->host, 103);
      su.sun_family = AF_UNIX;

      if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
         return(-1);

      if (clamp->verbose)
         printf("Trying %s...\n", clamp->host);

      if (connect(s, (struct sockaddr *)&su, sizeof(su)) != -1)
      {
         if (clamp->verbose)
            printf("Connected to %s.\n", clamp->host);
         //fcntl(s, F_SETFL, O_NONBLOCK);
         if (idsess)
         {
            if (clamp->verbose > 1)
               printf(">>> zIDSESSION\n");
            if ((write(s, "zIDSESSION", 11)) == -1)
            {
               close(s);
               return(-1);
            };
         };
         clamp->idsess = idsess;
         clamp->s = s;
         return(0);
      };
      close(s);
      return(-1);
   };
#endif

   // attempts IPv6 connection
#ifdef AF_INET6
   if ((hp = gethostbyname2(clamp->host, AF_INET6)))
      for(i = 0; hp->h_addr_list[i]; i++)
      {
         memset(&sa6, 0, sizeof(struct sockaddr_in6));
         memcpy(&sa6.sin6_addr, hp->h_addr_list[i], (size_t)hp->h_length);
         sa6.sin6_family = hp->h_addrtype;
         sa6.sin6_port   = port;
         //sa6.sin6_len    = sizeof(struct sockaddr_in6);

         if ((s = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
            return(-1);

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
            if (idsess)
            {
               if (clamp->verbose > 1)
                  printf(">>> zIDSESSION\n");
               if ((write(s, "zIDSESSION", 11)) == -1)
               {
                  close(s);
                  return(-1);
               };
            };
            clamp->idsess = idsess;
            clamp->s = s;
            return(0);
         };

         if (clamp->verbose)
            printf("connect to address %s: %s\n", addr, strerror(errno));

         close(s);
      };
#endif

   // attempts IPv4 connection
#ifdef AF_INET
   if ((hp = gethostbyname2(clamp->host, AF_INET)))
      for(i = 0; hp->h_addr_list[i]; i++)
      {
         memset(&sa, 0, sizeof(struct sockaddr_in));
         memcpy(&sa.sin_addr, hp->h_addr_list[i], (size_t)hp->h_length);
         sa.sin_family = hp->h_addrtype;
         sa.sin_port   = port;
         //sa.sin_len    = sizeof(struct sockaddr_in);

         if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            return(-1);

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
            if (idsess)
            {
               if (clamp->verbose > 1)
                  printf(">>> zIDSESSION\n");
               if ((write(s, "zIDSESSION", 11)) == -1)
               {
                  close(s);
                  return(-1);
               };
            };
            clamp->idsess = idsess;
            clamp->s = s;
            return(0);
         };

         if (clamp->verbose)
            printf("connect to address %s: %s\n", addr, strerror(errno));

         close(s);
      };
#endif

   return(-1);
}


/// recursively scans a directory without stopping if a virus is found
/// @param[in]  clamp    pointer to ClamAV Client session data
/// @param[in]  path     directory path to scan
int32_t clamavc_contscan(CLAMAVC * clamp, const char * path)
{
   char    buff[1024];
   ssize_t len;
   size_t  offset;
   size_t  nbyte;

   if (clamavc_connect(clamp, 0))
      return(-1);

   if ((len = snprintf(buff, 1023, "zCONTSCAN %s", path)) < 0)
      return(-1);
   if (len > 1023)
   {
      errno = ENOBUFS;
      return(-1);
   };
   buff[len] = '\0';
   nbyte = len + 1;

   if (clamp->verbose > 1)
      printf(">>> %s\n", buff);

   if ((write(clamp->s, buff, nbyte)) == -1)
   {
      clamavc_reset(clamp);
      return(-1);
   };

   if ((len = read(clamp->s, buff, 1023)) == -1)
   {
      clamavc_reset(clamp);
      return(-1);
   };
   buff[len] = '\0';
   if (clamp->verbose > 1)
      printf("<<< %s\n", buff);

   for(offset = 0; ( (buff[offset]) && (buff[offset] != ':') ); offset++);
   if (!(buff[offset]))
   {
      errno = EPROTO;
      return(-1);
   };
   offset += 2;

   if (clamp->verbose > 1)
      printf("<== %s\n", &buff[offset]);

   strncpy(clamp->msg, &buff[offset], CLAMAVC_MSG_LEN);
   if (!(strcmp(&buff[offset], "OK")))
   {
      clamavc_reset(clamp);
      return(0);
   };

   clamavc_reset(clamp);

   return(1);
}


/// returns last error code
/// @param[in]  clamp    pointer to ClamAV Client session data
const char * clamavc_error(CLAMAVC * clamp)
{
   if (clamp)
      return(clamp->msg);
   return("unknown status");
}


// converts from host byte order to network byte order
/// @param[in]  host     value to convert
/// @param[in]  width    byte width of value
uint32_t clamavc_hton(uint32_t host, size_t width)
{
   uint32_t mask;
   uint32_t num;

   mask = 0xFF;
   switch(width)
   {
      case 4:
         mask |= 0xFFFF0000;
      case 2:
         mask |= 0xFF00;
      default:
         host &= mask;
   };

   num = 1;
   if ( (*(uint8_t *)&num) != 1 )
      return(host);

   switch(width)
   {
      case 4:
         host = ((host << 16) & 0xFFFF0000) | ((host >> 16) & 0x0000FFFF);
      case 2:
         host = ((host <<  8) & 0xFF00FF00) | ((host >>  8) & 0x00FF00FF);
      default:
         break;
   };

   return(host);
}


/// initialize ClamAV client library session
CLAMAVC * clamavc_initialize(void)
{
   const char  * host;
   char        * data[10];
   CLAMAVC     * clamp;
   enum          env_vars
   {
      HOST,
      PORT,
      STREAMMAXLEN,
      VERBOSE
   };

   if (!(clamp = malloc(sizeof(CLAMAVC))))
   {
      errno = ENOMEM;
      return(NULL);
   };
   memset(clamp, 0, sizeof(CLAMAVC));

   // applies defaults and values from environment variables
   data[PORT]            = getenv("CLAMAVC_PORT");
   data[STREAMMAXLEN]    = getenv("CLAMAVC_STREAMMAXLEN");
   data[VERBOSE]         = getenv("CLAMAVC_VERBOSE");
   data[HOST]            = getenv("CLAMAVC_HOST");
   clamp->s              = -1;
   clamp->port           = data[PORT]          ? strtoul(data[PORT], NULL, 0)          : CLAMAVC_PORT;
   clamp->streamMaxLen   = data[STREAMMAXLEN]  ? strtoul(data[STREAMMAXLEN], NULL, 0)  : CLAMAVC_STREAMMAXLEN;
   clamp->verbose        = data[VERBOSE]       ? strtoul(data[VERBOSE], NULL, 0)       : CLAMAVC_VERBOSE;
   host                  = data[HOST]          ? data[HOST]                            : CLAMAVC_HOST;

   // save host name to data struct
   if (!(clamp->host = strdup(host)))
   {
      errno = ENOMEM;
      clamavc_close(clamp);
      return(NULL);
   };

   return(clamp);
}


/// recursively scans a directory using multiple threads
/// @param[in]  clamp    pointer to ClamAV Client session data
/// @param[in]  path     directory path to scan
int32_t clamavc_instream(CLAMAVC * clamp, const char * src, size_t nbyte)
{
   char     buff[1024];
   int32_t  offset;
   int32_t  len;
   uint32_t pos;
   uint32_t size;

   if (clamavc_connect(clamp, 1))
   {
      clamp->instream = 0;
      return(-1);
   };

   // send command to clamd if not already in a stream
   if (!(clamp->instream))
   {
      if (clamp->verbose > 1)
         printf(">>> zINSTREAM\n");
      if ((len = write(clamp->s, "zINSTREAM", 10)) == -1)
      {
         clamavc_reset(clamp);
         return(-1);
      };
   };
   clamp->instream = 1;

   // send data to clamd
   pos = 0;
   while(pos < nbyte)
   {
      if (clamp->verbose > 1)
         printf("==> +++data+++\n");

      size = ((nbyte - pos) < clamp->streamMaxLen) ? (nbyte - pos) : clamp->streamMaxLen;

      len = clamavc_hton(size, 4);
      if ((len = write(clamp->s, &len, sizeof(len))) == -1)
      {
         clamavc_reset(clamp);
         return(-1);
      };

      if ((len = write(clamp->s, &src[pos], size)) == -1)
      {
         clamavc_reset(clamp);
         return(-1);
      };

      pos += size;
   };
   if (nbyte)
      return(0);

   // send data stream terminator
   len = 0;
   if ((len = write(clamp->s, &len, sizeof(len))) == -1)
   {
      clamavc_reset(clamp);
      return(-1);
   };

   // read result from clamd
   clamp->instream = 0;
   if ((len = read(clamp->s, buff, 1023)) == -1)
   {
      clamavc_reset(clamp);
      return(-1);
   };
   buff[len] = '\0';
   if (clamp->verbose > 1)
      printf("<<< %s\n", buff);

   // process result from clamd
   for(offset = 0; ( (buff[offset]) && (buff[offset] != ':') ); offset++);
   if ((buff[offset]))
      for(offset = offset+1; ( (buff[offset]) && (buff[offset] != ':') ); offset++);
   if (!(buff[offset]))
   {
      clamavc_reset(clamp);
      errno = EPROTO;
      return(-1);
   };
   offset += 2;
   if (clamp->verbose > 1)
      printf("<== %s\n", &buff[offset]);
   strncpy(clamp->msg, &buff[offset], CLAMAVC_MSG_LEN);
   if (!(strcmp(&buff[offset], "OK")))
      return(0);

   return(1);
}


/// sends file opened with file handle as chuncked data to the server
/// @param[in]  clamp    pointer to ClamAV Client session data
/// @param[in]  file     file to scan
int32_t clamavc_instream_fildes(CLAMAVC * clamp, int fildes)
{
   int            len;
   char           buff[1024];

   if (lseek(fildes, (off_t)0, SEEK_SET) == -1)
      return(-1);

   while((len = read(fildes, buff, 1024)) > 0)
      if (clamavc_instream(clamp, buff, (unsigned)len))
         return(-1);

   if (len == -1)
      return(-1);

   return(clamavc_instream(clamp, NULL, 0));
}


/// sends file as chuncked data to the server
/// @param[in]  clamp    pointer to ClamAV Client session data
/// @param[in]  file     file to scan
int32_t clamavc_instream_file(CLAMAVC * clamp, const char * file)
{
   int            fd;
   int32_t        err;

   if ((fd = open(file, O_RDONLY)) == -1)
      return(-1);

   err = clamavc_instream_fildes(clamp, fd);

   close(fd);

   return(err);
}


/// returns daemon's version
const char * clamavc_libversion(void)
{
   return("libclamavc.la (" PACKAGE_NAME ") " PACKAGE_VERSION);
}


/// recursively scans a directory using multiple threads
/// @param[in]  clamp    pointer to ClamAV Client session data
/// @param[in]  path     directory path to scan
int32_t clamavc_multiscan(CLAMAVC * clamp, const char * path)
{
   char    buff[1024];
   ssize_t len;
   size_t  offset;
   size_t  nbyte;

   if (clamavc_connect(clamp, 0))
      return(-1);

   if ((len = snprintf(buff, 1023, "zMULTISCAN %s", path)) < 0)
      return(-1);
   if (len > 1023)
   {
      errno = ENOBUFS;
      return(-1);
   };
   buff[len] = '\0';
   nbyte = len + 1;

   if (clamp->verbose > 1)
      printf(">>> %s\n", buff);

   if ((len = write(clamp->s, buff, nbyte)) == -1)
   {
      clamavc_reset(clamp);
      return(-1);
   };

   if ((len = read(clamp->s, buff, 1023)) == -1)
   {
      clamavc_reset(clamp);
      return(-1);
   };
   buff[len] = '\0';
   if (clamp->verbose > 1)
      printf("<<< %s\n", buff);

   for(offset = 0; ( (buff[offset]) && (buff[offset] != ':') ); offset++);
   if (!(buff[offset]))
   {
      clamavc_reset(clamp);
      errno = EPROTO;
      return(-1);
   };
   offset += 2;

   if (clamp->verbose > 1)
      printf("<== %s\n", &buff[offset]);

   strncpy(clamp->msg, &buff[offset], CLAMAVC_MSG_LEN);
   if (!(strcmp(&buff[offset], "OK")))
   {
      clamavc_reset(clamp);
      return(0);
   };

   clamavc_reset(clamp);

   return(1);
}


/// checks the daemon's state
/// @param[in]  clamp    pointer to ClamAV Client session data
int32_t clamavc_ping(CLAMAVC * clamp)
{
   char    buff[1024];
   ssize_t len;

   if (clamavc_connect(clamp, 1))
      return(-1);

   if (clamp->verbose > 1)
      printf(">>> zPING\n");

   if ((len = write(clamp->s, "zPING", 6)) == -1)
   {
      clamavc_reset(clamp);
      return(-1);
   };

   if ((len = clamavc_read(clamp, buff, 1023)) == -1)
   {
      clamavc_reset(clamp);
      return(-1);
   };

   buff[len] = '\0';
   strncpy(clamp->msg, buff, CLAMAVC_MSG_LEN);
   if (clamp->verbose > 1)
      printf("<<< %s\n", buff);

   if ((strcmp(buff, "PONG")))
   {
      errno = EPROTO;
      return(-1);
   };

   return(0);
}


/// reads a data stream from the ClamAV daemon
/// @param[in]  clamp    pointer to ClamAV Client session data
/// @param[out] dst      pointer to read buffer
/// @param[in]  len      length of read buffer
int32_t clamavc_read(CLAMAVC * clamp, char * dst, size_t nbyte)
{
   size_t    pos;
   ssize_t  added;
   size_t   offset;
   size_t   len;

   if (!(clamp))
   {
      errno = EINVAL;
      return(-1);
   };

   pos   = 0;
   added = 0;

   if ((added = read(clamp->s, &dst[pos], nbyte-pos-1)) == -1)
   {
      return(-1);
   };
   pos += added;
   dst[pos] = '\0';

   if (!(pos))
      return(pos);

   while(dst[pos-1])
   {
      if ((added = read(clamp->s, &dst[pos], nbyte-pos-1)) == -1)
         return(-1);
      pos += added;
      if (pos >= (nbyte-1))
      {
         errno = ENOBUFS;
         return(-1);
      };
      dst[pos] = '\0';
   };
   len = pos;

   for(offset = 0; (dst[offset] && (dst[offset] != ' ')); offset++);
   if (!(dst[offset]))
   {
      errno = EPROTO;
      return(-1);
   };
   offset++;

   for(pos = offset; pos < len; pos++)
      dst[pos-offset] = dst[pos];

   len -= offset;

   return(len);
}


/// reloads the daemon's databases
/// @param[in]  clamp    pointer to ClamAV Client session data
int32_t clamavc_reload(CLAMAVC * clamp)
{
   char    buff[1024];
   ssize_t len;

   if (clamavc_connect(clamp, 0))
      return(-1);

   if (clamp->verbose > 1)
      printf(">>> zRELOAD\n");

   if ((len = write(clamp->s, "zRELOAD", 8)) == -1)
   {
      clamavc_reset(clamp);
      return(-1);
   };

   if ((len = read(clamp->s, buff, 1023)) == -1)
   {
      clamavc_reset(clamp);
      return(-1);
   };

   buff[len] = '\0';
   strncpy(clamp->msg, buff, CLAMAVC_MSG_LEN);
   if (clamp->verbose > 1)
      printf("<<< %s\n", buff);

   if ((strcmp(buff, "RELOADING")))
   {
      clamavc_reset(clamp);
      errno = EPROTO;
      return(-1);
   };

   clamavc_reset(clamp);

   return(0);
}


/// reset ClamAV client library session
/// @param[in]  clamp    pointer to ClamAV Client session data
int32_t clamavc_reset(CLAMAVC * clamp)
{
   if (!(clamp))
      return(errno = EINVAL);
   if (clamp->s != -1)
   {
      if (clamp->idsess)
      {
         if (clamp->verbose > 1)
            printf(">>> zEND\n");
         write(clamp->s, "zEND", 5);
      };
      close(clamp->s);
   };
   clamp->s        = -1;
   clamp->idsess   = 0;
   clamp->instream = 0;
   return(0);
}


/// scans a file or directory with archive support enabled
/// @param[in]  clamp    pointer to ClamAV Client session data
/// @param[in]  path     directory path to scan
int32_t clamavc_scan(CLAMAVC * clamp, const char * path)
{
   char    buff[1024];
   ssize_t len;
   size_t  offset;
   size_t  nbyte;

   if (clamavc_connect(clamp, 1))
      return(-1);

   if ((len = snprintf(buff, 1023, "zSCAN %s", path)) < 0)
      return(-1);
   if (len > 1023)
   {
      errno = ENOBUFS;
      return(-1);
   };
   buff[len] = '\0';
   nbyte = len + 1;

   if (clamp->verbose > 1)
      printf(">>> %s\n", buff);

   if ((len = write(clamp->s, buff, nbyte)) == -1)
   {
      clamavc_reset(clamp);
      return(-1);
   };

   if ((len = clamavc_read(clamp, buff, 1023)) == -1)
   {
      clamavc_reset(clamp);
      return(-1);
   };
   buff[len] = '\0';
   if (clamp->verbose > 1)
      printf("<<< %s\n", buff);

   for(offset = 0; ( (buff[offset]) && (buff[offset] != ':') ); offset++);
   if (!(buff[offset]))
   {
      errno = EPROTO;
      return(-1);
   };
   offset += 2;

   if (clamp->verbose > 1)
      printf("<== %s\n", &buff[offset]);

   strncpy(clamp->msg, &buff[offset], CLAMAVC_MSG_LEN);
   if (!(strcmp(&buff[offset], "OK")))
      return(0);

   return(1);
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

      case CLAMAVC_OSTREAMMAXLEN:
         clamp->streamMaxLen = CLAMAVC_STREAMMAXLEN;
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


/// shuts down the server
/// @param[in]  clamp    pointer to ClamAV Client session data
int32_t clamavc_shutdown(CLAMAVC * clamp)
{
   char    buff[1024];
   ssize_t len;

   if (clamavc_connect(clamp, 0))
      return(-1);

   if (clamp->verbose > 1)
      printf(">>> zSHUTDOWN\n");

   if ((len = write(clamp->s, "zSHUTDOWN", 10)) == -1)
   {
      clamavc_reset(clamp);
      return(-1);
   };

   if ((len = read(clamp->s, buff, 1023)) == -1)
   {
      clamavc_reset(clamp);
      return(-1);
   };

   buff[len] = '\0';
   if (clamp->verbose > 1)
      printf("<<< %s\n", buff);

   clamavc_reset(clamp);

   return(0);
}


/// returns daemon's version
/// @param[in]  clamp    pointer to ClamAV Client session data
const char * clamavc_version(CLAMAVC * clamp)
{
   ssize_t len;

   if (clamavc_connect(clamp, 1))
      return(NULL);

   if (clamp->verbose > 1)
      printf(">>> zVERSION\n");

   if ((len = write(clamp->s, "zVERSION", 9)) == -1)
   {
      clamavc_reset(clamp);
      return(NULL);
   };

   if ((len = clamavc_read(clamp, clamp->version, CLAMAVC_VER_LEN-1)) == -1)
   {
      clamavc_reset(clamp);
      return(NULL);
   };

   clamp->version[len] = '\0';
   strncpy(clamp->msg, clamp->version, CLAMAVC_MSG_LEN);
   if (clamp->verbose > 1)
      printf("<<< %s\n", clamp->version);

   return(clamp->version);
}


/* end of source code */
