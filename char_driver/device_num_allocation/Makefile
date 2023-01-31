# Makefile for Native Compilation 

obj-m := DynAlloc_dev_num.o 
KERN_DIR = /lib/modules/$(shell uname -r)/build/
PWD = $(shell pwd)

all:
	make -C $(KERN_DIR) M=$(PWD) modules 

clean:
	make -C $(KERN_DIR) M=$(PWD) clean 
