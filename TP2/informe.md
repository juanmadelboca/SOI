#Introduccion
En el siguiente trabajo se solicito realizar un bash, un interpretador de comandos. Los comandos seran introducidos por el usuario e interpretados por el bash.
A continuacion se responden los ejercicios orientativos para el desarrollo del bash.
#Ejercicios
1.¿Cómo ejecutamos un archivo que se llama exactamente como un builtin?

La manera de ejecutar un archivo que se llama exactamente como un builtin es utilizando el camino absoluto o relativo al archivo, ya que si queremos ejecutarlo con la búsqueda en secuencia en la variable de entorno PATH, primero se buscará en los comandos internos implementados en bash.

2.¿Por qué las recomendaciones de seguridad indican que es peligroso tener ./ en PATH al más puro estilo DOS?

Si se agrega './' al PATH, no será necesario anteponer ./ antes de ejecutar un archivo que se encuentre en el directorio actual, por lo tanto es inseguro tener ./ en el PATH ya que podría ejecutarse cualquier archivo sin anteponer el ./ antes del nombre del ejecutable.

3.Supongamos que existen 2 comandos posibles dentro de la secuencia que contiene PATH, donde el primero en la secuencia no está marcado como ejecutable y el segundo si. ¿Qué hace el intérprete bash, ejecuta el segundo o informa que el primero no tiene permiso de ejecución?

Falla por falta de permisos de ejecución.

4.Indique qué sucede cuando se tiene un directorio en el pwd actual con el mismo nombre que un archivo ejecutable en el PATH. ¿Dice que no puede ejecutar un directorio o ejecuta el comando que encontró?

Ejecuta el comando que encontró. Por ejemplo, si en el pwd actual creamos un directorio con el nombre sleep, al ejecutar el comando 'sleep' se ejecuta el comando que se encuentra al recorrer la secuencia de PATH. Para que aparezca el error de que no puede ejecutarse un directorio, el comando debe ser './sleep'.

5.¿Podemos poner directorios relativos en PATH?

Si, es posible.

6.Investiguen cuales son los comandos internos para manejo de procesos en background de bash.

Algunos de los comandos que se utilizan para ejecutar procesos en segundo plano son los siguientes:
<comando> & Ejecuta el proceso en segundo plano.
bg [PID] Lleva el proceso PID a background.
fg [PID] Lleva el proceso PID a foreground. En estos dos casos, el PID es el que se muestra con el comando bg o fg.

7.Investiguen como bash forma el árbol de procesos cuando ejecutamos cmd1 & cmd2 & cmd3 & ...&cmdN. Piensen la respuesta y luego utilicen pstree para corroborarla. Apliquen los resultados de su observación a baash.

Cuando se ejecuta, se ejecutan en Paralelo.

8.Indique cuantas letras 'a' debería imprimir el siguiente programa: fork(); fork(); fork(); printf("a");

Del primer fork salen 2 “a” ya que se crea una copia. Del segundo fork salen 2 mas “a” uno del original y otro por la copia que crea el primer fork.
Del tercero se imprimen 2 por cada uno, osea 4 veces. En total se imprimen 8 “a”.

9.Las corrientes estandar stdout y stderr están dirigidas ambas a la consola. ¿Cómo podemos utilizar los opeardores de redirección para separarlas?

Explicaremos esto con un ejemplo:
cat hello.txt > hola.txt lo que hace es redireccionar al archivo hola.txt
En forma generica:
(comando) > archivo

10.De más ejemplos de como usar la redireccion de entrada. Tenga cuidado que muchos programas deshabilitan la posibilidad de redirigir la entrada estandar a otra cosa que no sea una terminal.

Otro ejemplo es el siguiente(en forma generica):
(comando) < archivo
lo que hace esto es imprimir en pantalla (bash) lo que haga el comando con el archivo.
ejemplo:
head -5 < file.txt.

11.Los pipes existen en el filesystem mientras dura el comando. ¿Dónde se encuentran y que atributos tiene?

Las tuberías se emplean para interconectar procesos, es decir, podemos realizar varios procesos con una sola linea de comando. Su uso más habitual es pasar el resultado de la ejecución de un comando a la entrada de otro.