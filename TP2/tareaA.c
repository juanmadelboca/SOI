#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/wait.h>


#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDCYAN    "\033[1m\033[36m"
#define BLUE    "\033[34m"      /* Blue */
#define RESET   "\033[0m"
#define MAXARG 20
#define MAXCOM 100

extern char *cuserid(char *);

int readCommand(char* argv[][MAXARG], char* command,int argc[]);
int searchFile(char* path,char* arch, int recursive);
int searchBin(char* path, char* arch);
int checkCommand(char* command);

int main (){

	char command[MAXCOM];
	char* argV[2][MAXARG];
	int argC[2];
	int waitF,pid;
	char* paths[20];
	char hostname [20];
	char user[20];
	char executepath[200];
	char executepath2[200];
	char path[200];
	strcpy(path,getenv("HOME"));
	gethostname(hostname,20);
	cuserid(user);
	chdir(path);
	waitF=1;
	

	do
	{
		printf("\n%s%s@%s%s:",BOLDCYAN,user,hostname,RESET);
		printf("%s~%s$%s ",BLUE,getcwd(NULL,50),RESET );
		fgets(command,MAXCOM,stdin);

		if(!readCommand(argV,command,argC)){			// Chequeo si no se ingreso un pipe
			if (argC[0]==0)
				continue;
			strcpy(executepath,argV[0][0]);


			if(!strcmp(argV[0][argC[0]-1],"&")){
				argV[0][argC[0]-1]=NULL;
				waitF=0;
			} else
				waitF=1;

	    	if (!strcmp(argV[0][0],"cd")){			
				strcpy(path,argV[0][1]);
				chdir(path);
			} else if(checkCommand(executepath)){			//ver que se le pasa a search file,sino se encuentra no se ejecuta el if 

				pid = fork();				//creo el nuevo proceso
				if (pid<0) {				//si pid<0 da errror y termino el programa
					perror("Error en la creacion del hijo");
					return 1;
				}
				else if (pid == 0) {								//el hijo ejecuta esta sentencia el padre pasa de largo
					execv(executepath, argV[0]);						//execv ejecuta el binario con el nombre y opciones que trae argv y con el path que obtiene de search
				}
				
				if (waitF)
					waitpid(pid,NULL,0);
			}
		} else {									// Si se ingreso un pipe, hago lo siguiente

			int fd[2];
			pipe(fd);
			strcpy(executepath,argV[0][0]);
			strcpy(executepath2,argV[1][0]);

			if (((pid = fork()) != -1) && checkCommand(executepath)&& checkCommand(executepath2)) {
				if(pid == 0){         
				close(fd[0]);
				close(1);
				dup(fd[1]);
				close(fd[1]);
				execv(executepath, argV[0]); 

				} else {   
					          
					if((pid= fork())>=0)
					{
						if (pid==0){
							close(fd[1]);
							close(0);
							dup(fd[0]);
							close(fd[0]);
							printf("\n");
							execv(executepath2, argV[1]); 
						}
						
						/*char buffer[1024]="";
						read(fds,buffer,sizeof(buffer));
						printf("aca taaaaa: %s",buffer );*/
						wait(NULL);
					}
					else printf("ERROR: No se pudo crear el hijo" );
				}
				
			} else printf("ERROR: No se pudo ejecutar el pipe \n");	
		}

	}while (strcmp(command,"exit"));

	return 0;

}



/**
 * Lee el comando ingresado (command), y lo carga en argv[][], si no recibe el caracter de pipe '|' guarda
 * todos los comandos en la fila 0, sino, guarda los comandos luego del pipe en la fila 1.
 * @param char* command Comando a procesar.
 * @param char* argv[][] Array de palabras de command.
 * @param int argc[] numero de palabras
 * @return retorna un 1 si recibe un pipe y un 0 si no lo recibe.
 */
int readCommand(char* argv[][MAXARG], char* command,int argc[]){
  int words=0;
  int row=0;
  argv[row][words] = strtok(command, " \n");
  if (argv[row][words]!=NULL)
  for(words = 1; words < 20; words++){
    argv[row][words] = strtok(NULL , " \n");
    if (argv[row][words] == NULL)
      break;
    else if (!strcmp(argv[row][words],"|")){
      if (row){
        printf("Error: No se acepta mas de 1 pipe");
        exit(1);
      }
      argv[row][words]= NULL;
      row=1;
      argc[0]= words;
      words=-1;
    } 
  }
  if (row)
    argc[1]=words;
  else 
    argc[0]=words;

  return row;
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
      //printf ("ERROR: No existe el directorio %s\n",path);
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
 * Busca el archivo dentro de PATH, y redirecciona el puntero path a la dirección del archivo.
 * @param char* path Ruta a redireccionar. Ej "/home" --> "/bin/date"
 * @param char* arch Nombre el archivo a buscar
 * @return Devuelve 1 si encontro el archivo y un 0 si no lo encontro.
 */

int searchBin(char* path,char* arch){
  char *bin;
  char paths[200];
  sscanf(getenv("PATH"),"%200s",paths);
  bin= strtok(paths,":");
  while (bin!=NULL){
  	strcpy(path,bin);
  	if (searchFile(path,arch,0)){
  		return 1;
    }
    bin= strtok(NULL, ":");
  }
  return 0;
}