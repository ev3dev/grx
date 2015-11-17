m4_define([grx_major_version], [2])
m4_define([grx_minor_version], [99])
m4_define([grx_micro_version], [0])

m4_define([grx_version], [grx_major_version.grx_minor_version.grx_micro_version])

AC_PREREQ(2.69)

AC_INIT([grx], [grx_version], [david@lechnology.com])

AC_CONFIG_AUX_DIR([build])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADER([config.h])

# Checks for programs
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries
AC_CHECK_HEADER(jpeglib.h, [], [
  echo "Error! You need to have jpeglib.h around."
  exit -1
])
AC_CHECK_LIB(jpeg, main, [], [
  echo "Error! You need to have libjpeg around."
  exit -1
])
PKG_CHECK_MODULES([PNG], [libpng12])
#PKG_CHECK_MODULES([GLIB], [glib-2.0])

# Checks for typedefs, structures, and compiler characteristics
AC_PROG_CC_STDC

# Compiler flags
GRX_CFLAGS="$PNG_CFLAGS"
GRX_LIBS="$PNG_LIBS"

AM_INIT_AUTOMAKE([-Wall -Werror foreign -Wno-portability subdir-objects])
AM_PATH_GLIB_2_0
AM_PROG_AR
AM_SILENT_RULES([yes])
LT_INIT

# Versioning
GRX_MAJOR_VERSION=grx_major_version
GRX_MINOR_VERSION=grx_minor_version
GRX_MICRO_VERSION=grx_micro_version
GRX_VERSION=grx_version
AC_SUBST(GRX_MAJOR_VERSION)
AC_SUBST(GRX_MINOR_VERSION)
AC_SUBST(GRX_MICRO_VERSION)
AC_SUBST(GRX_VERSION)

# Introspection
#GOBJECT_INTROSPECTION_CHECK([1.38.0])

# Output
AC_SUBST(GRX_CFLAGS)
AC_SUBST(GRX_LIBS)

AC_CONFIG_FILES([Makefile grx-3.0.pc src/Makefile])

AC_OUTPUT
