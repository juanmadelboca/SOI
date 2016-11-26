all: main clean

main: main.o malloc.o 
	cc -o main main.o

main.o: main.c malloc.c
	cc -c -pedantic -Wall main.c

malloc.o: malloc.c
	cc -c -pedantic -Wall malloc.c

clean: 
	rm -f malloc.o main.o

