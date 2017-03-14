parse_test:test_parse.c sparse.c sapi.c sconstants.c sparse.h sapi.h sconstants.h stype.h
	gcc -Wall -g -o parse_test test_parse.c sparse.c sapi.c sconstants.c

eval_test:test_eval.c seval.c sparse.c sapi.c sconstants.c seval.h sparse.h sapi.h sconstants.h stype.h sbaselib.c slib.h sinit.c sinit.h siolib.c
	gcc -rdynamic -Wall -g -o eval_test test_eval.c seval.c sparse.c sapi.c sconstants.c sbaselib.c sinit.c siolib.c -ldl

smathlib.so:smathlib.c sapi.h sapi.c  sconstants.h sconstants.c stype.h
	gcc -Wall -g -fPIC -shared -o smathlib.so smathlib.c -lm
