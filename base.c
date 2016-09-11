#include "base.h"



struct base
{


	char cpuType[20];
	char cpuModel[100];
	char kernel[150];
	char upTime[60];
	const char *filesKernel;

	
};

void load(base *b){
  openFile("/proc/cpuinfo");
  search("vendor_id");
  strcpy(b->cpuType,search("vendor_id"));
  rewind(Fd);
  strcpy(b->cpuModel,search("model name"));
  fclose(Fd);

	openFile("/proc/version");
	strncpy(b->kernel,search("version")+8,15);
	fclose(Fd);

  

	 openFile("/proc/uptime");
	 strcpy(b->upTime,upTime(search(".")));
	 fclose(Fd);
}

void openFile(const char path[]) {
	Fd = fopen(path, "r");
	if (Fd == NULL) {
		printf("Error abriendo el fichero");
	} else {
    // printf("Fichero abierto \n");
	}
}

char* upTime(char texto[]){
  //toma la linea de texto y me la devuelve con el formato deseado
  float uptime,seconds;
  int days,hours,minutes;
  static char formatedUpTime[25];
  sscanf(texto,"%f ",&uptime);
  days=uptime/86400;
  hours=(uptime/3600)-(days*24);
  minutes=(uptime/60)-((days*24+hours)*60);
  seconds=uptime-((((days*24+hours)*60)+minutes)*60);
  snprintf(formatedUpTime, sizeof formatedUpTime, " %dD %d:%02d:%02.2f \n",days,hours,minutes,seconds);
  return formatedUpTime;

}


char* search(const char searchedWord[]) {
  //recibe una palabra de busqueda y devuelve un array con los datos 

  int tmp1, tmp2;
  static char* token;
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
          	if (!strcmp(searchedWord,".")||!strcmp(searchedWord,"version")){
          		token = strtok(text, " ");
				      token= strtok(NULL, "");
    			  return token;
          	}else{
          		token = strtok(text, ":");
				      token= strtok(NULL, "");
    			  return token;
          	}
          	
          }
        }
     }
    }

  }

}

void print(base *b){
	printf("Tipo CPU : %s", b->cpuType);
	printf("Modelo CPU : %s", b->cpuModel);
	printf("Kernel : %sc \n", b->kernel); //agrego una c aca porque cuando recorto justo en el espacio aparecen caracteres raros
  printf("UpTime : %s",b->upTime );
}


