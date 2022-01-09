CFLAGS=-g -O3
OBJS=calc.o ff.o z.o q.o y.tab.o main.o smain.o wmain.o
all: calc libcalc.a scalc wcalc
$(OBJS): calc.h
calc: $(OBJS)
	cc -g -o calc calc.o z.o q.o ff.o y.tab.o main.o -lgmp

libcalc.a: $(OBJS)
	ar cru libcalc.a calc.o z.o q.o ff.o y.tab.o

scalc: $(OBJS)
	cc -o scalc smain.o libcalc.a -lgmp

y.tab.c: cparse.y
	yacc -d cparse.y
clean:
	rm -f calc scalc wcalc *.o y.tab.* libcalc.a

wcalc: $(OBJS)
	cc -o wcalc wmain.o libcalc.a -lgmp
