obj-m := encriptador.o 
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	cc -o ioctl ioctl.c
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
