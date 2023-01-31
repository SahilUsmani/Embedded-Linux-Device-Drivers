/*
 * @file	mod_avg.c
 * @author	Sahil Usmani
 * @date	11 Jan 2023
 * @brief	A simple module program to find the average of two numbers
 * */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include "mod_add.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sahil Usmani");

static int num_1 = 10;
static int num_2 = 20;

static int __init start(void) {

	printk(KERN_ALERT "Module is Inserted\n");
	printk(KERN_ALERT "We're going to add now\n");
	printk(KERN_ALERT "The value of a is %d and b is %d\n", num_1, num_2);
	printk(KERN_ALERT "The addition is %d\n", my_add(num_1, num_2));
	return 0;
}

static void __exit end(void) {

	printk(KERN_ALERT "Module is Removed\n");
}

module_init(start);
module_exit(end);
