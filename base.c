#include "base.h"



struct base {
	char cpuType[20];
	char cpuModel[100];
	char kernel[150];
	char upTime[60];
	int fileSystems;
	char time[26];

	int processes;
	int context;
	char idleTime[60];
	char userCpuTime[60];
	char systemCpuTime[60];

	int totalMemory;
	int	freeMemory;
	int ioDisk;
	float loadAvg;

	void (*load)(base*);
	void (*print)(base*,int);

};

base* newBase(){
	base *b= (base*)malloc(sizeof(base));
	b->load= load;
	b->print= print;
	return b;
}


void load(base *b) {
	char temporal[150];
	int temp;
	char *p;


	openFile("/proc/cpuinfo");
	strcpy(b->cpuType, search("vendor_id"));
	rewind(Fd);
	strcpy(b->cpuModel, search("model name"));
	rewind(Fd);
	temp= atoi(search("cpu cores"));


	openFile("/proc/version");
	strcpy(b->kernel, search("version") + 13);
	p = strchr(b->kernel + 1, ' ');
	*p = '\0';
	

	openFile("/proc/uptime");
	strcpy(b->upTime, upTime(search("."),1));
	rewind(Fd);
	strtok(search("."), " ");
	strcpy(b->idleTime, upTime(strtok(NULL, " "),temp));
	strcpy(b->time,getTime());
	p = strchr(b->time, '\n');
	*p = '\0';
	

	openFile("/proc/filesystems");
	b->fileSystems = fileSystem();
	

	openFile("/proc/stat");
	strncpy(temporal, search("processes") + 10, 15);
	sscanf(temporal, "%d", &b->processes);
	rewind(Fd);
	strncpy(temporal, search("ctxt") + 5, 15);
	sscanf(temporal, "%d", &b->context);
	rewind(Fd);			
	strtok(search("cpu"), " ");		//devuelve en Jiffies (WTF) clicks de clock
	strcpy(temporal, strtok(NULL	, " "));				//hay que dividir por 100 aprox
	strcpy(b->userCpuTime, upTime(temporal,sysconf(_SC_CLK_TCK)*temp));
	strcpy(temporal, strtok(NULL, " "));
	strcpy(b->systemCpuTime,upTime(strtok(NULL, " "),sysconf(_SC_CLK_TCK)*temp));
	

	openFile("/proc/meminfo");
	sscanf(search("MemTotal"), "%d", &b->totalMemory);
	rewind(Fd);
	sscanf(search("MemFree"), "%d", &b->freeMemory);
	

	openFile("/proc/loadavg");
	sscanf(search(" "), "%f", &b->loadAvg);
	openFile("/proc/diskstats");
	sscanf(strchr(search("sda"),'a')+1, "%d", &b->ioDisk);

	fclose(Fd);

}

void openFile(const char path[]) {
	Fd = fopen(path, "r");
	if (Fd == NULL) {
		printf("Error abriendo el fichero");
	} else {
		//printf("Fichero abierto \n");
	}
}

char* upTime(char texto[],int div) {
	//toma la linea de texto y me la devuelve con el formato deseado
	float uptime, seconds;
	int days, hours, minutes;
	static char formatedUpTime[25];
	sscanf(texto, "%f ", &uptime);
	uptime= uptime/div;
	days = uptime / 86400;
	hours = (uptime / 3600) - (days * 24);
	minutes = (uptime / 60) - ((days * 24 + hours) * 60);
	seconds = uptime - ((((days * 24 + hours) * 60) + minutes) * 60);
	snprintf(formatedUpTime, sizeof formatedUpTime, " %dD %d:%02d:%02.2f \n",
			days, hours, minutes, seconds);
	return formatedUpTime;
}
	
int fileSystem() {
	int files = 0;	
	char text[150];
	while (feof(Fd) == 0) {

		fgets(text, 150, Fd); 
		files++;
	}
	return files;
}

char* getTime(){
	time_t timer;
  	struct tm * timeinfo;
	time (&timer);
 	timeinfo = localtime (&timer);
 	return asctime(timeinfo);

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

			if (searchedWord[0] == text[i]) {

				tmp1 = 0;
				tmp2 = i;

				while ((searchedWord[tmp1] == text[tmp2])
						&& (tmp2 < strlen(text))
						&& (tmp1 != strlen(searchedWord))) {

					tmp1++;
					tmp2++;

					if (tmp1 == strlen(searchedWord)) {
						if (!strcmp(searchedWord, ".")
								|| !strcmp(searchedWord, " ")
								|| !strcmp(searchedWord, "cpu")
								|| !strcmp(searchedWord, "version")
								|| !strcmp(searchedWord, "processes")
								|| !strcmp(searchedWord, "ctxt")
								|| !strcmp(searchedWord, "sda")) {
							return text;
						} else {
							token = strtok(text, ":");
							token = strtok(NULL, "");
							return token;
						}

					}
				}
			}
		}

	}
	return '\0';

}

void print(base *b, int opt) {
	if (opt==0){
		printf("\n------------------- %s -------------------\n\n", b->time);
		printf("Tipo CPU : %s", b->cpuType);
		printf("Modelo CPU : %s", b->cpuModel);
		printf("Kernel : %s \n", b->kernel);
		printf("UpTime : %s", b->upTime);
		printf("Cantidad de FS : %d\n", b->fileSystems);
	}else if (opt==1){
			printf("Procesos Creados : %d\n", b->processes);
			printf("Cambios de contexto : %d\n", b->context);
			printf("Idle CPU Time : %s", b->idleTime);
			printf("User CPU Time : %s", b->userCpuTime);
			printf("System CPU Time : %s", b->systemCpuTime);
		} else if (opt==2){
				printf("Memoria disponible / total: %d / %d\n",b->freeMemory,b->totalMemory );
				printf("Peticiones a Disco : %d \n", b->ioDisk);
				printf("Carga Promedio : %.2f \n", b->loadAvg);
				printf("----------------------------------------------------------------\n");
			}
	
}

