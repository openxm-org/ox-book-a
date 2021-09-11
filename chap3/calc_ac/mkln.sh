#!/bin/sh

for i in calc.c calc.h cmain.c cparse.y ff.c main.c q.c smain.c z.c; do 
     if [ ! -f $i ]; then
		 ln -s ../../chap2/calc/$i .
	 fi
done


