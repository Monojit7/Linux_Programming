obj-m+=ebbchar.o

INC = /usr/src/linux-headers-3.19.0-84/include/ 
all:
	 make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	 $(CC) testebbchar.c -o test
clean:
	 make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	 rm test
	 rm *rc*
