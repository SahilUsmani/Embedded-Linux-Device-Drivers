/*
 * @file	mod_add.c
 * @author	Sahil Usmani
 * @date	11 Jan 2023
 * @brief	This is a module program to add two numbers and also export the functionality to be used 
 * 		by other modules as well
 * */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sahil Usmani");

int my_add_mod(int a, int b) {

	return (a + b);
}

/*
 * Below is the macro for Exporting your function to The Kernel Symbol Table and make it available to be used by 
 * other modules.
 * It takes only one argument, the name of the function.*/
EXPORT_SYMBOL(my_add_mod);

static int __init start(void) {
	
	printk(KERN_ALERT "Module is Inserted\n");
	return 0;
}

static void __exit end(void) {

	printk(KERN_ALERT"Module is Removed\n");
}

module_init(start);
module_exit(end);
