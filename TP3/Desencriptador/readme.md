# Para hacer funcionar #
---

- Ir a la consola a la carpeta /Desencriptador 
- Escribir "make"  
- Luego escribir "sudo insmod desencriptador.ko" para cargar el modulo.  
- Escribir "sudo mknod desencrip c 109 0" para crear el archivo del dispositivo.  
- Por ultimo ejecutar ./ioctl para abrir el manejador del modulo.  
- Para eliminar el modulo escribir "sudo rmmod desencriptador"  

---
