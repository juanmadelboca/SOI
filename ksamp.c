#include "ksamp.h"

int main(int argc, char* argv[]) {

  struct base b;

  int option = 0;
  int flag=0;
  int interval;
  // :2 no se muere pero como gestiono el nuevo parametro
  while (1) { //tengo que poner los cases y si llevan mas argumentos : (por cada uno?)
    flag = getopt(argc, argv, "l:s");
    if (flag==-1){
      break;
    }
    switch (flag) {
    case 'l':
      interval = atoi(optarg); //me trae el primer parametro
      printf("%s %d", "\n ----> OPCION L-> Parametro 1: ", interval);
      interval = atoi(optarg + strlen(optarg) + 1);
      printf("%s %d \n\n", "   Parametro 2: ", interval);
      option=2;
      break;
    case 's':
      printf("\n----> OPCION S \n\n");
      option=1;
      break;

    default:
      printf("Error 404: Not Found \n");
    }

  }

  //while (1) {
    b.load(option);
   // for (int i = 0; i < 30000000; ++i);
  //  system("clear");
    b.print(option);
  //}
  return 0;
}

