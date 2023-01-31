#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

MODULE_AUTHOR("Sahil Usmani");
MODULE_LICENSE("GPL"); // Tells Module bear free License
MODULE_VERSION("5.15.84-v7l+") // This is RaspberryPi 4 model b kernel version for cross-compilation
MODULE_DESCRIPTION("Hello World Module");

/*
 * This is driver initialization entry point and is called during system startup(if the driver
 * is statically compiled into the kernel) or when the module is inserted into the kernel using
 * sudo insmod <kernel object file>
 * */
static int __init helloModule_init(void) {

	printk(KERN_ALERT "Hello World!\nInserting Module into Kernel\n");
	return 0;
}

/*
 * This is the driver exit point and is called when unloading a module from linux kernel using
 * sudo rmmod <kernel object file named>
 * This function has no effect if the driver is statically compiled into the kernel */
static void __exit helloModule_exit(void) {

	printk(KERN_ALERT "This is a clean-up function\n");
	printk(KERN_ALERT "Good bye...\nRemoving Module from Kernel\n");
}

/* These functions are declared in the linux/module.h header file */
module_init(helloModule_init);  // Insert Module into Kernel
module_exit(helloModule_exit);  // Remove Module from Kernel
