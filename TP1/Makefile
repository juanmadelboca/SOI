all: ksamp clean

ksamp: ksamp.o base.o 
	cc -o ksamp ksamp.o

ksamp.o: ksamp.c ksamp.h base.c
	cc -c -pedantic -Wall ksamp.c

base.o: base.c base.h
	cc -c -pedantic -Wall base.c

clean: 
	rm -f base.o ksamp.o

