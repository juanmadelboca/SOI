#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

FILE *Fd;
static char text[150];
void openFile(const char[]);
char* search(const char[]);
void ksamp();

int main(int argc, char* argv[]) {

  int option = 0;
  int interval;
  ksamp();
  // :2 no se muere pero como gestiono el nuevo parametro
  while ((option = getopt(argc, argv, "l:s")) != -1) { //tengo que poner los cases y si llevan mas argumentos : (por cada uno?)
    switch (option) {
    case 'l':
      interval = atoi(optarg); //me trae el primer parametro
      printf("%s %d \n", "ksamp pro con parametros", interval);
      break;
    case 's':
      printf("ksamp con agregado \n");
      break;

    default:
      printf("comando erroneo \n");
    }
  }

  return 0;
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
  printf("%s",search("."));
  fclose(Fd);
}

char* search(const char searchedWord[]) {
  //recibe una palabra de busqueda y devuelve un array con los datos 

  int tmp1, tmp2;
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
            return text;
        }
      }
    }
  }

}