CFLAGS=-g -O3
OBJS=calc.o ff.o z.o q.o y.tab.o
all: calc
$(OBJS): calc.h
calc: $(OBJS)
	cc -g -o calc calc.o z.o q.o ff.o y.tab.o -lgc -lgmp
y.tab.c: cparse.y
	yacc -d cparse.y
clean:
	rm -f calc *.o y.tab.*
