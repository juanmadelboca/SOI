all: baash clean
baash: baash.o 
	cc -o baash baash.o

baash.o: baash.c 
	cc -c -pedantic -Wall baash.c

clean: 
	rm -f baash.o 

