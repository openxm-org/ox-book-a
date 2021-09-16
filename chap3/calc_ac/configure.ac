AC_PREREQ([2.69])
AC_INIT([calc],[1.0])
AC_PROG_CC
AC_PROG_YACC
GCLIB=""
AC_ARG_WITH([gc],[  --with-gc          use GC [[yes]]],
[with_gc=${withval}],[with_gc=yes])
if test "${with_gc}" != no; then
    GCLIB="-lgc"
    AC_DEFINE([USEGC])
    AC_SUBST([GCLIB])
fi
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
