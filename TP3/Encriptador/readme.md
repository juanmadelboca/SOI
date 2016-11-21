# Para hacer funcionar #
---

- Ir a la consola a la carpeta /Encriptador  
- Escribir "make"  
- Luego escribir "sudo insmod encriptador.ko" para cargar el modulo.  
- Escribir "sudo mknod encrip c 105 0" para crear el archivo del dispositivo.  
- Por ultimo ejecutar ./ioctl para abrir el manejador del modulo.  
- Para eliminar el modulo escribir "sudo rmmod encriptador"  

---

