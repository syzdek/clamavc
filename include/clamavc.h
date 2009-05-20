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
 *  @file include/clamavc.h libclamav.la API
 */
#ifndef _CLAMAVC_H
#define _CLAMAVC_H 1

//////////////
//          //
//  Macros  //
//          //
//////////////

/*
 * The macros "BEGIN_C_DECLS" and "END_C_DECLS" are taken verbatim
 * from section 7.1 of the Libtool 1.5.14 manual.
 */
/* BEGIN_C_DECLS should be used at the beginning of your declarations,
   so that C++ compilers don't mangle their names. Use END_C_DECLS at
   the end of C declarations. */
#undef BEGIN_C_DECLS
#undef END_C_DECLS
#if defined(__cplusplus) || defined(c_plusplus)
#   define BEGIN_C_DECLS  extern "C" {    ///< exports as C functions
#   define END_C_DECLS    }               ///< exports as C functions
#else
#   define BEGIN_C_DECLS  /* empty */     ///< exports as C functions
#   define END_C_DECLS    /* empty */     ///< exports as C functions
#endif


/*
 * The macro "PARAMS" is taken verbatim from section 7.1 of the
 * Libtool 1.5.14 manual.
 */
/* PARAMS is a macro used to wrap function prototypes, so that
   compilers that don't understand ANSI C prototypes still work,
   and ANSI C compilers can issue warnings about type mismatches. */
#undef PARAMS
#if defined (__STDC__) || defined (_AIX) \
        || (defined (__mips) && defined (_SYSTYPE_SVR4)) \
        || defined(WIN32) || defined (__cplusplus)
# define PARAMS(protos) protos   ///< wraps function arguments in order to support ANSI C
#else
# define PARAMS(protos) ()       ///< wraps function arguments in order to support ANSI C
#endif


/*
 * The following macro is taken verbatim from section 5.40 of the GCC
 * 4.0.2 manual.
 */
#if __STDC_VERSION__ < 199901L
# if __GNUC__ >= 2
# define __func__ __FUNCTION__
# else
# define __func__ "<unknown>"
# endif
#endif


// Exports function type
// Based on the Macros from OpenLDAP's library header.
#ifdef WIN32
#   ifdef OTADM_LIBS_DYNAMIC
#      define OTADM_F(type)   extern __declspec(dllexport) type   ///< used for library calls
#      define OTADM_V(type)   extern __declspec(dllexport) type   ///< used for library calls
#   else
#      define OTADM_F(type)   extern __declspec(dllimport) type   ///< used for library calls
#      define OTADM_V(type)   extern __declspec(dllimport) type   ///< used for library calls
#   endif
#else
#   ifdef OTADM_LIBS_DYNAMIC
#      define OTADM_F(type)   type                                ///< used for library calls
#      define OTADM_V(type)   type                                ///< used for library calls
#   else
#      define OTADM_F(type)   extern type                         ///< used for library calls
#      define OTADM_V(type)   extern type                         ///< used for library calls
#   endif
#endif


///////////////
//           //
//  Headers  //
//           //
///////////////

#include <inttypes.h>
#include <sys/types.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////

// ClamAV Client defaults
#define CLAMAVC_HOST               "localhost"
#define CLAMAVC_PORT               3310
#define CLAMAVC_SOCKET             NULL
#define CLAMAVC_STREAMMAXLEN       4096

// ClamAV Client options
#define CLAMAVC_OHOST              0x01
#define CLAMAVC_OPORT              0x02
#define CLAMAVC_OSOCKET            0x03
#define CLAMAVC_OSTREAMMAXLEN      0x04
#define CLAMAVC_OVERBOSE           0x05


/////////////////
//             //
//  Datatypes  //
//             //
/////////////////

/// internal libclamavc.la state data
typedef struct clamavc CLAMAVC;


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
BEGIN_C_DECLS

// closes ClamAV client library session and frees resources
OTADM_F(void) clamavc_close PARAMS((CLAMAVC * clamp));

// recursively scans a directory without stopping if a virus is found
//OTADM_F(int32_t) clamavc_contscan PARAMS((CLAMAVC * clamp,
// const char * dir));

// recursively scans a directory without stopping if a virus is found
//OTADM_F(int32_t) clamavc_contscan_s PARAMS((CLAMAVC * clamp,
// const char * dir));

// frees memory allocated by library
OTADM_F(char *) clamavc_free PARAMS((CLAMAVC * clamp, void * ptr));

// initialize ClamAV client library session
OTADM_F(CLAMAVC *) clamavc_initialize PARAMS((void));

// sends chuncked data to the server
OTADM_F(int32_t) clamavc_instream PARAMS((CLAMAVC * clamp, uint8_t * src,
   uint32_t len));

//OTADM_F(int32_t) clamavc_multiscan PARAMS((CLAMAVC * clamp));

// checks the daemon's state
OTADM_F(int32_t) clamavc_ping PARAMS((CLAMAVC * clamp));

// scans a file or directory without archive support enabled
OTADM_F(int32_t) clamavc_rawscan PARAMS((CLAMAVC * clamp, const char * path));

// reloads the daemon's databases
OTADM_F(int32_t) clamavc_reload PARAMS((CLAMAVC * clamp));

// scans a file or directory with archive support enabled
OTADM_F(int32_t) clamavc_scan PARAMS((CLAMAVC * clamp, const char * path));

// sets library options
OTADM_F(int32_t) clamavc_set_opt PARAMS((CLAMAVC * clamp, uint32_t opt,
   const void * valp));

// shuts down the server
OTADM_F(int32_t) clamavc_shutdown PARAMS((CLAMAVC * clamp));

//OTADM_F(int32_t) clamavc_stats PARAMS((CLAMAVC * clamp));

// returns daemon's version
OTADM_F(char *) clamavc_version PARAMS((CLAMAVC * clamp));

END_C_DECLS
#endif /* end of header */
