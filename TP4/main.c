#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "malloc.c"




int main (){

	int b;
	
	void *mem;
	
	printf("Tamaño del bloque: %i Bytes\n",BLOCK_SIZE);
	
	printf("Ingrese la cantidad de bloques: ");
	scanf("%i",&b);
  
    int bloques[b];
	
	for (int i=0;i<b;i++){
		printf("\n Ingrese la dimencion del bloque %d : ",i+1);
		scanf("%i",&bloques[i]);
		
		mem =(double*) malloc(bloques[i]);//llamado a la funcion malloc de la clase bloque 
		printf( "Se asigna posicion de memoria : %p \n ",mem) ;
} 
	
	
    free(mem);

	return 0;
}


	
	