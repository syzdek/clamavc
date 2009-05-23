#
#   Clam Antivirus Client Library
#   Copyright (C) 2009 Alaska Communications Systems Group, Inc
#
#   @ACS_LICENSE_HEADER_START@
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program; if not, write to the Free Software Foundation, Inc.,
#   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
#   @ACS_LICENSE_HEADER_END@
#
#   acinclude.m4 - custom m4 macros used by configure.ac
#


# AC_CLAMAVC_GCC_WARNINGS()
# -----------------------------------
AC_DEFUN([AC_CLAMAVC_GCC_WARNINGS],[dnl

   # prerequists
   AC_REQUIRE([AC_PROG_CC])

   # sets compiler
   if test "x${CC}" = "x";then
      ac_clamavc_gcc_warnings_cc=cc;
     else
      ac_clamavc_gcc_warnings_cc=${CC};
   fi

   # list of args
   ac_clamavc_gcc_warnings_argslist=""
   ac_clamavc_gcc_warnings_argslist="$ac_clamavc_gcc_warnings_argslist -W"
   ac_clamavc_gcc_warnings_argslist="$ac_clamavc_gcc_warnings_argslist -Wall"
   ac_clamavc_gcc_warnings_argslist="$ac_clamavc_gcc_warnings_argslist -Werror"

   # creates test source code
   AC_LANG_CONFTEST(
      [AC_LANG_PROGRAM(
         [[int main(void);]],
         [[return(0);]]
      )])

   # loops through options
   for ac_clamavc_gcc_warnings_arg in $ac_clamavc_gcc_warnings_argslist;do

      # prints flag to test
      AC_MSG_CHECKING(for gcc flag $ac_clamavc_gcc_warnings_arg)

      # check cache
      ac_clamavc_gcc_warnings_test="no"
      for ac_cflag_arg in $ac_cv_clamavc_gcc_warnings_args;do
         if test "x$ac_clamavc_gcc_warnings_arg" = "x$ac_cflag_arg";then
            ac_clamavc_gcc_warnings_test="yes"
         fi
      done

      # perform test
      if test "x$ac_clamavc_gcc_warnings_test" = "xno";then
         $ac_clamavc_gcc_warnings_cc $CFLAGS_WARNINGS $ac_clamavc_gcc_warnings_arg conftest.c -o conftest.o > /dev/null 2>&1
         if test "x$?" = "x0";then
            ac_clamavc_gcc_warnings_args="$ac_clamavc_gcc_warnings_args $ac_clamavc_gcc_warnings_arg"
            AC_MSG_RESULT(yes)
           else
            AC_MSG_RESULT(no)
         fi
        else
         ac_clamavc_gcc_warnings_args="$ac_clamavc_gcc_warnings_args $ac_clamavc_gcc_warnings_arg"
         AC_MSG_RESULT((cached) yes)
      fi
   done

   # adds flags to CFLAGS_WARNINGS
   AC_MSG_CHECKING(for duplicate warning flags)
   for ac_clamavc_gcc_warnings_arg in $ac_clamavc_gcc_warnings_args;do
      ac_clamavc_gcc_warnings_test="no"
      for ac_cflag_arg in $CFLAGS_WARNINGS;do
         if test "x$ac_clamavc_gcc_warnings_arg" = "x$ac_cflag_arg";then
            ac_clamavc_gcc_warnings_test="yes"
         fi
      done
      if test "x$ac_clamavc_gcc_warnings_test" = "xno";then
         CFLAGS_WARNINGS="$CFLAGS_WARNINGS $ac_clamavc_gcc_warnings_arg"
      fi
   done
   AC_MSG_RESULT(done)

   # cache results
   ac_cv_clamavc_gcc_warnings_args=$ac_clamavc_gcc_warnings_args
   AC_CACHE_SAVE()
])dnl


# AC_CLAMAVC_GCC_STRICT_WARNINGS()
# -----------------------------------
AC_DEFUN([AC_CLAMAVC_GCC_STRICT_WARNINGS],[dnl

   # prerequists
   AC_REQUIRE([AC_PROG_CC])
   AC_REQUIRE([AC_CLAMAVC_GCC_WARNINGS])

   # sets compiler
   if test "x${CC}" = "x";then
      ac_clamavc_gcc_strict_cc=cc;
     else
      ac_clamavc_gcc_strict_cc=${CC};
   fi

   # list of args
   ac_clamavc_gcc_strict_argslist=""
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -W"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wall"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Werror"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -std=gnu99"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -pedantic"
   #ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wtraditional"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wpragma-once"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wextra-tokens"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wnewline-eof"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wsequence-point"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wdiv-by-zero"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wfloat-equal"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wdisabled-optimization"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wchar-subscripts"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wcomment"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wformat"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wimplicit"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wmain"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wmissing-braces"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wparentheses"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wreturn-type"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wswitch"
   #ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wswitch-default"	# for some reason any call to memset() fails with this flag
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wtrigraphs"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wunused"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wunknown-pragmas"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wuninitialized"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wmultichar"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wundef"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wendif-labels"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wshadow"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wlarger-than-4096"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wpointer-arith"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wbad-function-cast"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wcast-qual"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wcast-align"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wwrite-strings"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wconversion"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wsign-compare"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Waggregate-return"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wstrict-prototypes"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wstrict-aliasing"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wmissing-prototypes"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wmissing-declarations"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wmissing-noreturn"
   #ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wmissing-format-attribute"	# vprintf is failing this warning
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wredundant-decls"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wunreachable-code"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Winvalid-pch"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wpacked"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wpadded"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wnested-externs"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wenum-clash"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Winline"
   ac_clamavc_gcc_strict_argslist="$ac_clamavc_gcc_strict_argslist -Wlong-long"

   # creates test source code
   AC_LANG_CONFTEST(
      [AC_LANG_PROGRAM(
         [[int main(void);]],
         [[return(0);]]
      )])

   # loops through options
   for ac_clamavc_gcc_strict_arg in $ac_clamavc_gcc_strict_argslist;do

      # prints flag to test
      AC_MSG_CHECKING(for gcc flag $ac_clamavc_gcc_strict_arg)

      # checks cache
      ac_clamavc_gcc_strict_test="no"
      for ac_cflag_arg in $ac_cv_clamavc_gcc_strict_args;do
         if test "x$ac_clamavc_gcc_strict_arg" = "x$ac_cflag_arg";then
            ac_clamavc_gcc_strict_test="yes"
         fi
      done

      # perform test
      if test "x$ac_clamavc_gcc_strict_test" = "xno";then
         $ac_clamavc_gcc_strict_cc $CFLAGS_WARNINGS $ac_clamavc_gcc_strict_arg conftest.c -o conftest.o > /dev/null 2>&1
         if test "x$?" = "x0";then
            ac_clamavc_gcc_strict_args="$ac_clamavc_gcc_strict_args $ac_clamavc_gcc_strict_arg"
            AC_MSG_RESULT(yes)
           else
            AC_MSG_RESULT(no)
         fi
        else
         ac_clamavc_gcc_strict_args="$ac_clamavc_gcc_strict_args $ac_clamavc_gcc_strict_arg"
         AC_MSG_RESULT((cached) yes)
      fi
   done

   # adds flags to CFLAGS_WARNINGS
   AC_MSG_CHECKING(for duplicate warning flags)
   for ac_clamavc_gcc_strict_arg in $ac_clamavc_gcc_strict_args;do
      ac_clamavc_gcc_strict_test="no"
      for ac_cflag_arg in $CFLAGS_WARNINGS;do
         if test "x$ac_clamavc_gcc_strict_arg" = "x$ac_cflag_arg";then
            ac_clamavc_gcc_strict_test="yes"
         fi
      done
      if test "x$ac_clamavc_gcc_strict_test" = "xno";then
         CFLAGS_WARNINGS="$CFLAGS_WARNINGS $ac_clamavc_gcc_strict_arg"
      fi
   done
   AC_MSG_RESULT(done)

   # cache results
   ac_cv_clamavc_gcc_strict_args=$ac_clamavc_gcc_strict_args
   AC_CACHE_SAVE()
])dnl


# AC_CLAMAVC_OPTION_DEBUG()
AC_DEFUN([AC_CLAMAVC_OPTION_DEBUG],[dnl

   # define options
   enableval=""
   AC_ARG_ENABLE(
      debug,
      [AS_HELP_STRING([--enable-debug], [enable debug mode])],
      [ USE_DEBUG=$enableval ],
      [ USE_DEBUG=$enableval ]
   )

   # logic
   if test "x$USE_DEBUG" != "xno" && test "x$USE_DEBUG" != "x";then
      DEBUG=1
      USE_DEBUG='yes'
      AC_DEFINE_UNQUOTED(DEBUG, 1, [Enable Debug Options])
      AC_DEFINE(DEBUG, 1, [debug level])
     else
      USE_DEBUG='no'
      DEBUG=0
   fi

   # export substitutions
   AC_SUBST(USE_DEBUG, "$USE_DEBUG")
   AC_SUBST(DEBUG, "$DEBUG")
])dnl


# AC_CLAMAVC_OPTION_WARNINGS()
AC_DEFUN([AC_CLAMAVC_OPTION_WARNINGS],[dnl

   # define options
   enableval=""
   AC_ARG_ENABLE(
      warnings,
      [AS_HELP_STRING([--disable-warnings], [disable compiler warnings])],
      [ USE_WARNINGS=$enableval ],
      [ USE_WARNINGS=$enableval ]
   )
   enableval=""
   AC_ARG_ENABLE(
      strictwarnings,
      [AS_HELP_STRING([--enable-strictwarnings], [enable strict compiler warnings (for developers) [default=no]])],
      [ USE_STRICTWARNINGS=$enableval ],
      [ USE_STRICTWARNINGS=$enableval ]
   )

   # logic
   if test "x$USE_WARNINGS" != "xno";then
      AC_CLAMAVC_GCC_WARNINGS
      USE_WARNINGS='yes'
      if test "x$USE_STRICTWARNINGS" = "xyes";then
         AC_CLAMAVC_GCC_STRICT_WARNINGS
         USE_STRICTWARNINGS='yes'
        else
         USE_STRICTWARNINGS='no'
      fi
     else
      USE_STRICTWARNINGS='no'
   fi

   # export substitutions
   AC_SUBST(USE_WARNINGS, "$USE_WARNINGS")
   AC_SUBST(USE_STRICTWARNINGS, "$USE_STRICTWARNINGS")
   AC_SUBST(CFLAGS_WARNINGS, "$CFLAGS_WARNINGS")
])dnl


# end of M4 file
