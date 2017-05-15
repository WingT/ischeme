dir_inc=./include
dir_src=./src
dir_lib=./lib
src=$(wildcard ${dir_src}/*.c)
inc=${wildcard ${dir_inc}/*.h}
CFLAGS=-Wall -I${dir_inc}
scheme:${src} ${include} ${dir_lib}/sbaselib.c ${dir_lib}/siolib.c
	gcc -rdynamic ${CFLAGS} -o scheme ${src} ${dir_lib}/sbaselib.c ${dir_lib}/siolib.c -ldl

smathlib.so:${dir_lib}/smathlib.c ${dir_src}/sapi.c ${dir_src}/sconstants.c  ${dir_inc}/sconstants.h ${dir_inc}/stype.h ${dir_inc}/sapi.h
	gcc ${CFLAGS} -fPIC -shared -o smathlib.so ${dir_lib}/smathlib.c -lm
clean:
	rm -f scheme && rm -f *.so
