#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

int searchFile(char* path, int recursive);
int searchBin(char* path);

int main (void)
{
  char b[100]= "/home/franco/SOI/TP2/ls";
  

  if (searchFile(b,1)){
    printf("La encontre en: %s\n",b );
  } else if (searchBin(b)){
    printf("La encontre en: %s\n", b);
  } else
  printf("No la encontre");






  return 0;
}

int searchFile(char* path,int recursive){
  DIR * pDir;
  char* arch;
  char auxPath[50];

  arch = strchr(path, '\0');              //Con este algoritmo obtengo el nombre
  while (*arch != '/')                    //del archivo al que quiero acceder
    arch--;                               //y obtengo la ruta por otro lado.
  *arch= '\0';
  arch++;
  strcpy(auxPath,arch);

  pDir = opendir (path);

  if (! pDir) {
      printf ("ERROR: No existe el directorio %s",path);
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
  strcat(path,"/");
  strcat(path,auxPath);

  return 0;
}

int searchBin(char* path){
  char bin[50]="/bin/";
  char* arch;

  arch = strchr(path, '\0');              //Con este algoritmo obtengo el nombre
  while (*arch != '/')                    //del archivo al que quiero acceder
    arch--;                               //y obtengo la ruta por otro lado.
  *arch= '\0';
  arch++;
  strcpy(path,bin);
  strcat(path,arch);

  if (searchFile(path,0)){
    return 1;
  }
  return 0;
}