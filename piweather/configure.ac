#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([piweather], [0.1], [david.daniel@students.ffhs.ch])
AC_CONFIG_SRCDIR([src/device/UidRegistry.cpp])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([gnu -Wall])
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_AR
LT_INIT

AX_CXX_COMPILE_STDCXX_11([noext],[mandatory])
AX_PTHREAD

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h netdb.h netinet/in.h stddef.h stdint.h stdlib.h string.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([gethostbyname gettimeofday memmove memset socket strdup])

AC_CONFIG_FILES([\
                 Makefile \
                 build/Makefile \
                 ])
AC_OUTPUT
