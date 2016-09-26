##introduccion:

se desarrollo un programa en lenguaje C, que permite obtener informacion del
sistema mediante la lectura y parceo de los archivos en /proc

##Desarrollo:

Como primer paso se procedio a localizar la informacion solicitada. Viendo que 
todos eran archivos que se debian abrir y luego buscar alguna informacion se crearon
dos funciones search y openFile.

#Parte A:

los datos se obtuvieron de los siguientes ficheros:

Tipo y modelo de CPU		**/proc/cpuinfo**
Version del kernel		**/proc/version**
Tiempo de inicio del sistema	**/proc/uptime**
Tiempos de CPU,Fecha de		**/proc/stat**
inicio, procesos creados
memoria en uso y disponible	**/proc/meminfo**
carga promedio			**/proc/loadavg**
Lecturas/escrituras a disco	**/proc/diskstats**

#Parte B:
tipo y modelo se obtuvieron directamente de cpuinfo, parseando 
desde los ":".
La version de linux se debio tomar la informacion central de la 
version omitiendo el comienzo y final.
tiempo de inicio de sistema y de iddle, se tomaron los valores
desde uptime, el primero para el tiempo y el otro para iddle y 
se lo transformo a un float para luego reestructurarlo de la forma
requerida. Para esto se creo una funcion llamada upTime que tomaba una cadena de
caracteres y lo convertia en un string con formato DD HH:MM:SS.SS.

#Parte C:

Los tiempos de cpu, se obtuvieron de stats, y fueron parseados tambien con upTime 
La cantidad de cambios de contexto y procesos creados, se parsearon de la misma
manera que los tipo y modelo de cpu

Parte D:
La memoria libre y total se obtuvo de meminfo, tomando los valores despues de los ":".
Las peticiones al disco fueron tomadas de diskstats analizando los valores de lecturas


Para la realizacion de los puntos se conformo una estructura donde se guardaban los datos, a modo de base 
de datos y luego se hizo una funcion que imprima lo requerido por el usuario. Para parsear los comandos de 
el usuario en la terminal se utilizo la libreria getopt y la respectiva funcion.

##Conclusion

Por medio de este trabajo se logro obtener datos del sistema operativo, e interactuar por medio de funciones prearmadas con el mismo
ademas se profundizo el manejo de C, y se utilizaron herramientas como cppcheck y doxygen, para realizar analisis estatico del 
codigo y generar documentacion de forma automatica, a su vez por medio del makefile se compilo el proyecto y se aprendio los procesos
de la compilacion.
