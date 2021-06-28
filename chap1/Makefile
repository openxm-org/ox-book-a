usage:
	@echo "make all, make clean"
all: myplot

glib4.o: glib4.c glib4.h
	cc -c glib4.c
obj0-lib.o: obj0-lib.c obj0-lib.h
	cc -c obj0-lib.c
minicomp2.o: minicomp2.c obj0-lib.h glib4.h
	cc -c minicomp2.c
myplot: glib4.o minicomp2.o obj0-lib.o
	cc -o myplot minicomp2.o glib4.o obj0-lib.o -lX11 -lm

clean:
	-rm *.o myplot *~

