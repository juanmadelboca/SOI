#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

FILE *Fd;
void openFile(const char[]);
char* search(const char[]);
void ksamp();
char * upTime(char[]);

int main(int argc, char* argv[]) {
 

  int option = 0;
  int interval;
  // :2 no se muere pero como gestiono el nuevo parametro
  while ((option = getopt(argc, argv, "l:s")) != -1) { //tengo que poner los cases y si llevan mas argumentos : (por cada uno?)
    switch (option) {
    case 'l':
      interval = atoi(optarg); //me trae el primer parametro
      printf("%s %d \n", "ksamp pro con parametros", interval);
      interval = atoi(optarg+strlen(optarg)+1);
      printf("%s %d \n", " otro parametro ", interval);
      break;
    case 's':
      printf("ksamp con agregado \n");
      break;

    default:
      printf("comando erroneo \n");
    }
  }
  while(1){

  ksamp();
  }

  return 0;
}


char* upTime(char texto[]){
  //toma la linea de texto y me la devuelve con el formato deseado
  float uptime;
  int days,hours,minutes,seconds;
  static char formatedUpTime[25];
  sscanf(texto,"%f ",&uptime);
  days=uptime/86400;
  hours=(uptime/3600)-(days*24);
  minutes=(uptime/60)-((days*24+hours)*60);
  seconds=uptime-((((days*24+hours)*60)+minutes)*60);
  snprintf(formatedUpTime, sizeof formatedUpTime, "UpTime: %dD %d:%d:%d \n",days,hours,minutes,seconds);
  return formatedUpTime;

}
void openFile(const char path[]) {
  Fd = fopen(path, "r");
  if (Fd == NULL) {
    printf("Error abriendo el fichero");
  } else {
    // printf("Fichero abierto \n");
  }
}

void ksamp() {
  //gestiona los pedidos de busqueda, apertura y cierre de la version original del ksamp
  openFile("/proc/cpuinfo");
  printf("%s",search("vendor_id"));
  rewind(Fd);
  printf("%s",search("model name"));
  fclose(Fd);

  openFile("/proc/version");
  printf("%s",search("version"));
  fclose(Fd);

  openFile("/proc/uptime");
  printf("%s",upTime(search(".")));
  fclose(Fd);
  for (int i = 0; i < 300000000; ++i);
  {
    /* code */
  }
  system("clear");
}

char* search(const char searchedWord[]) {
  //recibe una palabra de busqueda y devuelve un array con los datos 

  int tmp1, tmp2;
  static char text[150];
  while (feof(Fd) == 0)

  {
    fgets(text, 150, Fd); //lee una linea de el archivo

    for (int i = 0; i < strlen(text); i++) {

      if (searchedWord[0] == text[i])

      {

        tmp1 = 0;

        tmp2 = i;

        while ((searchedWord[tmp1] == text[tmp2])
            && (tmp2 < strlen(text))
            && (tmp1 != strlen(searchedWord)))

        {
          tmp1++;

          tmp2++;

          if (tmp1 == strlen(searchedWord))
          {
            return text;
          }
        }
     }
    }

  }

}