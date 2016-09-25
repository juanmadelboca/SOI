#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <unistd.h>


FILE *Fd;
void openFile(const char[]);
char* search(const char[]);
char* upTime(char[],int);
int fileSystem();
typedef struct base base;
void load(base *b);
void print(base *b, int opt);
char* getTime();