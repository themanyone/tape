AC_INIT([tape],[0.1],[bug-automake@gnu.org])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
LT_PREREQ([2.2])
AM_PROG_AR
LT_INIT([dlopen])
LT_INIT
AC_PROG_CC
AC_C_CONST

LT_INIT
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])

AC_ARG_WITH([xz],
	[AS_HELP_STRING([--with-xz], [Enable lzma xz compression])],
	[],
	[with_xz=yes])

AM_CONDITIONAL([WITH_LZMA], [test x$with_xz = xyes])

AS_IF([test x$with_xz = xyes],
[AC_DEFINE(WITH_LZMA,[1],[Use lzma xz compression])
#pkg-config
PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([LZMA], [liblzma])
])

AC_OUTPUT
