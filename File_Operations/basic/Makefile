
obj-m := sudf.o
KERN_DIR = /lib/modules/$(shell uname -r)/build/
PWD = $(shell pwd)

all:
	make -C $(KERN_DIR) M=$(PWD) modules
	gcc -o app1 app1.c

clean:
	make -C ${KERN_DIR} M=$(PWD) clean
	rm -rf app1
