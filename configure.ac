AC_INIT([duplitrace], [0.1], [you@example.com])
AM_INIT_AUTOMAKE([foreign subdir-objects])
#AC_CONFIG_SRCDIR([src/duplitrace_cli/main.cpp])
AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX

# Options
AC_ARG_ENABLE([cli],
  [AS_HELP_STRING([--disable-cli], [Do not build the CLI frontend])],
  [], [enable_cli=yes])

AC_ARG_ENABLE([service],
  [AS_HELP_STRING([--disable-service], [Do not build the duplitrace service])],
  [], [enable_service=yes])

AC_ARG_ENABLE([unittests_service],
  [AS_HELP_STRING([--disable-unittests_service],
                  [Do not build the duplitrace service unittests])],
                  [], [enable_service_unittests=yes])

AM_CONDITIONAL([BUILD_CLI], [test "x$enable_cli" = "xyes"])
AM_CONDITIONAL([BUILD_SERVICE], [test "x$enable_service" = "xyes"])
AM_CONDITIONAL([BUILD_SERVICE_UNITTESTS], [test "x$enable_service_unittests" = "xyes"])

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  src/duplitrace_cli/Makefile
  src/duplitrace_indexer/Makefile
  src/unittests_service/Makefile
])
AC_OUTPUT
