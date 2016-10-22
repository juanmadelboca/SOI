#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDCYAN    "\033[1m\033[36m"
#define BLUE    "\033[34m"      /* Blue */
#define RESET   "\033[0m"

int leerEntrada(char* argv[], char* command);
int getPaths(char* paths[]);

int main (){
		char command[20];
		char* argV[20];
		int argC;	
		char exit[6]="exit";
		char* paths[20];
		char hostname [20];
    	char user[20];
		int	pathCounter;

		gethostname(hostname,20);
    	cuserid(user);

		pathCounter=getPaths(paths);
	do
	{
		printf("%s%s@%s%s:",BOLDCYAN,user,hostname,RESET);
		printf("%s~%s$ ",BLUE,getenv("PATH") );
		scanf("%s",command);
		argC=leerEntrada(argV,command);
	}while (strcmp(command,exit));

}

int leerEntrada(char* argv[], char* command){

	int words = 0;	
	argv[0] = strtok(command, " \n");
	for(words = 1; words < 20; words++){
		argv[words] = strtok(NULL , " \n");
		if (argv[words] == NULL)
			break;
	}
	return words;
}
int getPaths(char* paths[]){
	//acomoda los path del sistema en un arreglo de paths y devuelve la cantidad de paths
	int pathCounter;
	char* pathVar = getenv("PATH");
	
	paths[0] = strtok(pathVar, ":");
	for(pathCounter = 1; pathCounter < 20; pathCounter++){
		paths[pathCounter] = strtok(NULL,":");		
		if (paths[pathCounter] == NULL)
			break;
	}
	
	strtok(NULL,":");
	return pathCounter+1;
}

						