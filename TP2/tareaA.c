#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDCYAN    "\033[1m\033[36m"
#define BLUE    "\033[34m"      /* Blue */
#define RESET   "\033[0m"

int leerEntrada(char* argv[], char* command);
int getPaths(char* paths[]);
int searchFile(char* path,int recursive);

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

    	int a= chdir(getenv("HOME"));

    	printf("%d\n",a );

		pathCounter=getPaths(paths);
	do
	{
		printf("%s%s@%s%s:",BOLDCYAN,user,hostname,RESET);
		printf("%s~%s$%s ",BLUE,getcwd(NULL,50),RESET );
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

/**
 * Busca el archivo dentro del path y redirecciona el puntero enviado.
 * @param char* path Ruta + archivo a buscar. Ej "/bin/date"
 * @param int recursive Enviar un 0 para busqueda local y un 1 para busqueda recursiva
 * @return Devuelve 1 si encontro el archivo y un 0 si no lo encontro.
 */

int searchFile(char* path,int recursive){
	DIR * pDir;
	char* arch;
	char auxPath[50];

	arch = strchr(path, '\0');							//Con este algoritmo obtengo el nombre
	while (*arch != '/')								    //del archivo al que quiero acceder
		arch--;											          //y obtengo la ruta por otro lado.
	*arch= '\0';
	arch++;
	strcpy(auxPath,arch);

  pDir = opendir (path);

  if (! pDir) {
      printf ("Error al abrir el directorio %s",path);
      return 0;
  }

  while (1) {
      struct dirent * file;
      file = readdir (pDir);
      if (! file){
          break;
      }else if(!strcmp(file->d_name,arch)){             //Chequeo si esta en esa carpeta
      	strcat(path,"/");
      	strcat(path,file->d_name);
      	return 1;
      } else if (file->d_type == DT_DIR && recursive && strcmp(file->d_name,"..") && strcmp(file->d_name,".") 
      			&& strchr(file->d_name,'.')==NULL){        //Si esta habilitada la busqueda recursiva, busco en otras carpetas
      	strcat(path,"/");
      	strcat(path,file->d_name);
      	strcat(path,"/");
      	strcat(path,auxPath);
      	if (searchFile(path,recursive)){                 // Si lo encontro, retorno 1
      		return 1;
      	} else {                                      //Sino, elimino la carpeta que agregue, y vuelvo a buscar
      		char* p = strchr(path, '\0');							
				  while (*p != '/')
            p--;										
				  *p= '\0';
      	}   
      }
  }
	return 0;
}
						