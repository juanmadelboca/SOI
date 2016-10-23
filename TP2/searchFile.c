#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

int searchFile(char* path,char* arch, int recursive);
int searchBin(char* path, char* arch);

int main (void)
{
  char path[100]= "/home";
  char arch[50]= "tareaA";
  

  if (searchFile(path,arch,1)){
    printf("La encontre en: %s\n",path );
  } else if (searchBin(path,arch)){
    printf("La encontre en: %s\n", path);
  } else
  printf("No la encontre");


  return 0;
}


int searchFile(char* path,char* arch,int recursive){
  DIR * pDir;
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

int searchBin(char* path,char* arch){
  char bin[50]="/bin";
  strcpy(path,bin);

  if (searchFile(path,arch,0)){
    return 1;
  }
  return 0;
}