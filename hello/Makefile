# Makefile for Native Compilation 
obj-m := hello.o 
KERN_DIR = /lib/modules/5.15.0-56-generic/build/
# PWD = $(shell pwd)

all:
	make -C $(KERN_DIR) M=$(PWD) modules 

clean:
	make -C $(KERN_DIR) M=$(PWD) clean 
