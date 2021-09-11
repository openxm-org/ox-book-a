AC_PREREQ([2.69])
AC_INIT([calc],[1.0])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([foreign])
AM_MAINTAINER_MODE
LT_INIT
AC_PROG_CC
AC_PROG_YACC

AC_ARG_ENABLE([static_calc],
[  --enable-static-calc    force to link statically with libcalc. [[no]]],
[enable_static_calc=${enableval:=yes}],[enable_static_calc=no])
AM_CONDITIONAL([CALC_STATIC],[test "${enable_static_calc:=no}" != no])

use_cygwin=no
case "$host" in
*cygwin*)
	use_cygwin=yes
    AC_DEFINE([USE_CYGWIN])
    ;;
esac
AM_CONDITIONAL([USE_CYGWIN],[test "${use_cygwin}" = yes])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT











