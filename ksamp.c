#include "ksamp.h"

int main(int argc, char* argv[]) {

  struct base b;

  int option = 0;
  int interval;
  // :2 no se muere pero como gestiono el nuevo parametro
  while ((option = getopt(argc, argv, "l:s")) != -1) { //tengo que poner los cases y si llevan mas argumentos : (por cada uno?)
    switch (option) {
    case 'l':
      interval = atoi(optarg); //me trae el primer parametro
      printf("%s %d \n", "ksamp pro con parametros", interval);
      interval = atoi(optarg + strlen(optarg) + 1);
      printf("%s %d \n", " otro parametro ", interval);
      break;
    case 's':
      printf("ksamp con agregado \n");
      break;

    default:
      printf("comando erroneo \n");
    }
  }

  while (1) {
    b.load();
    for (int i = 0; i < 30000000; ++i);
    system("clear");
    b.print();
  }
  return 0;
}

