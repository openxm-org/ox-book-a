CFLAGS=-g -O3
OBJS=calc.o ff.o z.o q.o y.tab.o main.o smain.o
all: calc libcalc.a scalc
$(OBJS): calc.h
calc: $(OBJS)
	cc -g -o calc calc.o z.o q.o ff.o y.tab.o main.o -lgc -lgmp

libcalc.a: $(OBJS)
	ar cru libcalc.a calc.o z.o q.o ff.o y.tab.o

scalc: $(OBJS)
	cc -o scalc smain.o libcalc.a -lgc -lgmp

y.tab.c: cparse.y
	yacc -d cparse.y
clean:
	rm -f calc scalc *.o y.tab.* libcalc.a
