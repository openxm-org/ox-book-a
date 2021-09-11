bin_PROGRAMS = calc
calc_SOURCES = main.c calc.h
calc_DEPENDENCIES = libcalc.la
if CALC_STATIC
    calc_LDADD = -lgc -lgmp ./.libs/libcalc.a
else
    calc_LDADD = -lgc -lgmp ./.libs/libcalc.la
endif
lib_LTLIBRARIES = libcalc.la
libcalc_la_SOURCES = calc.c ff.c z.c q.c cparse.y calc.h
CLEANFILES = cparse.c
