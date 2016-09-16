#include "base.h"

void fload(struct base *b, int opt);
void fprint(struct base *b, int opt);

struct base {
	char cpuType[20];
	char cpuModel[100];
	char kernel[150];
	char upTime[60];
	int fileSystems;

	int processes;
	int context;
	char idleTime[60];
	char userCpuTime[60];
	char systemCpuTime[60];

	int totalMemory;
	int	freeMemory;
	float loadAvg;

	void (*pload)(struct base *,int);
	void (*pprint)(struct base *,int);
	base() {
		pload = fload;
		pprint = fprint;
	}
	void load(int opt) {
		pload(this, opt);
	}
	void print(int opt) {
		pprint(this, opt);
	}
};

void fload(struct base *b, int opt) {
	char temporal[150];
	char *p;

	switch(opt){
		case 2:
		case 1:
		case 0:
			openFile("/proc/cpuinfo");
			strcpy(b->cpuType, search("vendor_id"));
			rewind(Fd);
			strcpy(b->cpuModel, search("model name"));
			fclose(Fd);

			openFile("/proc/version");
			strcpy(b->kernel, search("version") + 13);
			p = strchr(b->kernel + 1, ' ');
			*p = '\0';
			fclose(Fd);

			openFile("/proc/uptime");
			strcpy(b->upTime, upTime(search(".")));
			rewind(Fd);
			strcpy(temporal, strtok(search("."), " "));
			strcpy(temporal, strtok(NULL, " "));
			strcpy(b->idleTime, upTime(temporal));
			fclose(Fd);

			openFile("/proc/filesystems");
			b->fileSystems = fileSystem();
			fclose(Fd);

			openFile("/proc/stat");
			strncpy(temporal, search("processes") + 10, 15);
			sscanf(temporal, "%d", &b->processes);
			rewind(Fd);
			strncpy(temporal, search("ctxt") + 5, 15);
			sscanf(temporal, "%d", &b->context);
			rewind(Fd);			
			strcpy(temporal, strtok(search("cpu"), " "));		//devuelve en Jiffies (WTF) clicks de clock
			strcpy(temporal, strtok(NULL, " "));				//hay que dividir por 100 aprox
			strcpy(b->userCpuTime, upTime(temporal));
			strcpy(temporal, strtok(NULL, " "));
			strcpy(temporal, strtok(NULL, " "));
			strcpy(b->systemCpuTime,upTime(temporal));
			fclose(Fd);

			openFile("/proc/meminfo");
			sscanf(search("MemTotal"), "%d", &b->totalMemory);
			rewind(Fd);
			sscanf(search("MemFree"), "%d", &b->freeMemory);
			fclose(Fd);

			openFile("/proc/loadavg");
			sscanf(search(" "), "%f", &b->loadAvg);
			fclose(Fd);
			break;
	}

}

void openFile(const char path[]) {
	Fd = fopen(path, "r");
	if (Fd == NULL) {
		printf("Error abriendo el fichero");
	} else {
		//printf("Fichero abierto \n");
	}
}

char* upTime(char texto[]) {
	//toma la linea de texto y me la devuelve con el formato deseado
	float uptime, seconds;
	int days, hours, minutes;
	static char formatedUpTime[25];
	sscanf(texto, "%f ", &uptime);
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
								|| !strcmp(searchedWord, "ctxt")) {
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

}

void fprint(struct base *b, int opt) {
	printf("Tipo CPU : %s", b->cpuType);
	printf("Modelo CPU : %s", b->cpuModel);
	printf("Kernel : %s \n", b->kernel);
	printf("UpTime : %s", b->upTime);
	printf("Cantidad de FS : %d\n", b->fileSystems);
	printf("Procesos : %d\n", b->processes);
	printf("Cambios de contexto : %d\n", b->context);
	printf("IdleTime : %s", b->idleTime);

	printf("User CPU Time : %s", b->userCpuTime);
	printf("System CPU Time : %s", b->systemCpuTime);

	printf("Memoria disponible / total: %d / %d\n",b->freeMemory,b->totalMemory );
	printf("Carga Promedio : %.2f \n", b->loadAvg);
	
	if (opt>0){
		printf("\n--------- Imprimo opcion S --------\n");
		if (opt==2){
			printf("\n--------- Imprimo opcion L --------\n");
		}
	}
	
}

