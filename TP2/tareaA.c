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
#define MAXARG 20
#define MAXCOM 100

int readCommand(char* argv[], char* command);
int searchFile(char* path,char* arch, int recursive);
int searchBin(char* path, char* arch);
int checkCommand(char* command);

int main (){

	char command[MAXCOM];
	char* argV[MAXARG];
	int argC,pid;	
	char exit[6]="exit";
	char* paths[20];
	char hostname [20];
	char user[20];
	char executepath[200];
	char path[200];
	strcpy(path,getenv("HOME"));
	gethostname(hostname,20);
	cuserid(user);
	chdir(path);

	do
	{
		printf("%s%s@%s%s:",BOLDCYAN,user,hostname,RESET);
		printf("%s~%s$%s ",BLUE,getcwd(NULL,50),RESET );
		fgets(command,MAXCOM,stdin);
		argC=readCommand(argV,command);

		strcpy(executepath,argV[0]);
    if (!strcmp(argV[0],"cd")){			
			strcpy(path,argV[1]);
			chdir(path);
		} else if(checkCommand(executepath)){			//ver que se le pasa a search file,sino se encuentra no se ejecuta el if 

			pid = fork();				//creo el nuevo proceso
			if (pid<0) {				//si pid<0 da errror y termino el programa
				perror("Error en la creacion del hijo");
				return 1;
			}
			else if (pid == 0) {								//el hijo ejecuta esta sentencia el padre pasa de largo
				execv(executepath, argV);						//execv ejecuta el binario con el nombre y opciones que trae argv y con el path que obtiene de search
				printf("%s\n", "soy hijo y entre al nano" );
			}

				wait();		//el padre espera a los procesos hijos que crea para evitar que queden zombies
				printf("%s\n", "termine" );
		}
	}while (strcmp(command,exit));

}

/**
 * Lee el comando ingresado (command), lo carga en argv[] y devuelve el número de palabras del mismo.
 * @param char* command Comando a procesar.
  * @param char* argv[] Array de palabras de command.
 * @return Numero de palabras de command.
 */
int readCommand(char* argv[], char* command){
  int words = 0;  
  argv[0] = strtok(command, " \n");
  for(words = 1; words < 20; words++){
 	argv[words] = strtok(NULL , " \n");
    if (argv[words] == NULL)
      break;
  }
  return words;
}
/**
 * Chequea la validez del comando, si es valido, realiza la acción, sino imprime error.
 * @param char* command Comando a ejecutar por baash.
 * @return Devuelve 1 si el comando es valido y un 0 si no es valido.
 */

 /**
 * COMANDOS VALIDOS
 * > /carpeta1/carpeta2/../archivo : Busca solamente en esa dirección absoluta el archivo.
 * > ./archivo : Busca a partir de la carpeta actual, en todas las subcarpetas hasta encontrar el archivo.
 * > archivo: Busca solamente en la carpeta actual y en los binarios al archivo.
 */
int checkCommand(char* command){
  char* p= command;
  char file[50];

  if (*p=='.' && (*p+1)=='/'){
    strcpy(file,(p+2));
    strcpy(command,getcwd(NULL,50));
    return searchFile(command,file,1);
  } else if (*p=='/'){
    p=strchr(command,'\0');
    while(*p!='/')
      p--;
    *p= '\0';
    p++;
    strcpy(file,p);
    return searchFile(command,file,0);
  } else {
    while(*p!='\0'){
      p++;
      if (*p=='/'){
        printf("Error de sintaxis\n");
        return 0;
      }
    }
    strcpy(file,command);
    strcpy(command,getcwd(NULL,50));
    if(searchFile(command,file,0)){
      return 1;
    } else if (searchBin(command,file)){
      return 1;
    } else {
      printf("No se encontro el archivo\n" );
    }

  }
  return 0;
}

/**
 * Busca el archivo arch dentro del path y redirecciona el puntero path a la dirección donde se encuentra
 * el archivo.
 * @param char* path Ruta del archivo a buscar. Ej: "/home"--> "/home/usr/imagenes/imagen.png"
 * @param char* arch Nombre el archivo a buscar
 * @param int recursive Enviar un 0 para busqueda local y un 1 para busqueda recursiva
 * @return Devuelve 1 si encontro el archivo y un 0 si no lo encontro.
 */

int searchFile(char* path,char* arch,int recursive){
  DIR * pDir;
  pDir = opendir (path);

  if (! pDir) {
      printf ("ERROR: No existe el directorio %s\n",path);
      return 0;
  }

  while (1) {
      struct dirent * file;
      file = readdir (pDir);
      if (! file){
          break;
      }else if(!strcmp(file->d_name,arch)&&file->d_type != DT_DIR){             //Chequeo si esta en esa carpeta
        strcat(path,"/");
        strcat(path,file->d_name);
        return 1;
      } else if (file->d_type == DT_DIR && recursive && strcmp(file->d_name,"..") && strcmp(file->d_name,".") 
            && strchr(file->d_name,'.')==NULL){        //Si esta habilitada la busqueda recursiva, busco en otras carpetas
        strcat(path,"/");
        strcat(path,file->d_name);
        if (searchFile(path,arch,recursive)){                 // Si lo encontro, retorno 1
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

/**
 * Busca el archivo dentro de la carpeta /bin, y redirecciona el puntero path a la dirección del archivo.
 * @param char* path Ruta a redireccionar. Ej "/home" --> "/bin/date"
 * @param char* arch Nombre el archivo a buscar
 * @return Devuelve 1 si encontro el archivo y un 0 si no lo encontro.
 */

int searchBin(char* path,char* arch){
  char bin[50]="/bin";
  strcpy(path,bin);

  if (searchFile(path,arch,0)){
    return 1;
  }
  return 0;
}