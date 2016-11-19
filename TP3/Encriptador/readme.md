# Para hacer funcionar #
---

- Ir a la consola a /Encriptador  
- Escribir "make"  
- Luego escribir "sudo insmod encriptador.ko" para cargar el modulo.  
- Escribir "sudo mknod encrip c 105 0" para crear el archivo del dispositivo.  
- Por ultimo ejecutar ./ioctl para encriptar  
- Para eliminar el modulo "sudo rmmod encriptador"  
---

# MUY IMPORTANTE #

### Cuando hagas el desencriptador cambiale el MAJOR_NUM por otro, ese va cuando creas el archivo del dispositivo ###
