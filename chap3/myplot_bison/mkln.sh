#!/bin/sh

for i in glib4.c glib4.h obj0-lib.c obj0-lib.h; do
	if [ ! -f $i ]; then
		ln -s ../../chap1/$i .
	fi
done
