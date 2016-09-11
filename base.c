#include "base.h"



struct base
{


	const char *cpuType;
	const char *cpuModel;
	const char *kernel;
	const char *upTime;
	const char *filesKernel;

	
};

void load(){
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
  snprintf(formatedUpTime, sizeof formatedUpTime, "UpTime: %dD %d:%02d:%02.2f \n",days,hours,minutes,seconds);
  return formatedUpTime;

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
          	char* token = strtok(text, searchedWord);
			while (token) {
    			
    			token = strtok(NULL, "");
    			printf("token: %s\n", token);
			}	
            return text;
          }
        }
     }
    }

  }

}


