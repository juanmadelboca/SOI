#include "ksamp.h"

int main(int argc, char* argv[]) {

  base *b= newBase();

  int option;
  int interval[2];

  int flag = getopt(argc, argv, "l:sh");
  option=0;

  switch (flag) {
    case 'l':
      interval[0] = atoi(optarg); //me trae el primer parametro
      interval[1] = atoi(optarg + strlen(optarg) + 1);
      option=2;
      break;
    case 's':
      option=1;
      break;

    case '?':
      printf("Utiliza ksamp -h para ayuda\n");
      option=-1;
      break;

    case 'h':
      printf("Utiliza ksamp [opcion]\n");
      printf("Opciones:\n");
      printf("-s Imprime cambios de contexto y tiempos del CPU\n");
      printf("-l [intervalo] [duracion] Imprime peticiones a disco");
      printf(",tiempos de carga del CPU y memoria disponible en ");
      printf("el tiempo especificado  \n");
      break;
    }
  
  if (getopt(argc, argv, "l:sh")!=-1){
      printf("Error: No se admite mas de una opcion\n");
      printf("Utiliza ksamp -h para ayuda\n");
      option=-1;
    }


  if (option==0){
    b->load(b);
    b->print(b,0);
  } else if (option==1){
    b->load(b);
    b->print(b,0);
    b->print(b,option);
  } else if (option==2){
    b->load(b);
    b->print(b,0);
    b->print(b,1);
    for (int i=0; i<(interval[1]/interval[0]);i++){
      b->load(b);
      b->print(b,option);
      sleep(interval[0]);
    }
  }

  return 0;
}

