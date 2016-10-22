#include <stdio.h>
#include <string.h>

int leerEntrada(char* argv[], char* command);

int main (){
		char command[20];
		char* argV[20];
		int argC;
		char exit[6]="exit";
	do
	{
		printf("%s\n",getenv("PATH") );
		scanf("%s",command);
		argC=leerEntrada(argV,command);
		for (int i = 0; i < argC; ++i)
		{
		printf("%s \n",argV[i]);
		}
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

						