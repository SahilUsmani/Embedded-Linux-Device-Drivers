/*******************************************************************************************
 * @file	module_parameter.c
 * @author	Sahil Usmani
 * @date	11 Jan 2023
 * @version	0.1
 ******************************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
// #include <linux/moduleparam.h>
#include <linux/kern_levels.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sahil Usmani");
MODULE_DESCRIPTION("A simple parameter passing program");
MODULE_VERSION("0.1");

/*
 * Variables are declared as static to keep their scope local to this module ..
 * and avoid namespace pollution.
 * */
static char* charvar;
static int intvar = 0;
static int arr_value[3] = {0};
static int cb_value = 0;

/*************************************** module_param() *******************************************
 * The following Macro is used to mention that these variables can be initialised from command line
 * Variables are enabled to be modified from command line
 *
 * module_param(variable_name, type of varaible, permission);
 *
 * charp = character pointer 
 * bool = boolean 
 * invbool = inverted Boolean, Rest are same data types
 * */
module_param(charvar, charp, S_IRUGO);				// for char value
module_param(intvar, int, S_IRUGO);				// for int value

/*************************************** module_param_array() ************************************/
/*
 * The following macro is used tp pass the array as an argument, where the values are supplied as a 
 * comma - seperated list, are also supported by the module loader.
 *
 * module_param_array(variable_name, type, num, permission);
 *
 * num is an integer variable it is optional otherwise NULL
 *
 *************************************************************************************************/
module_param_array(arr_value, int, NULL, S_IRUSR | S_IWUSR);	// for array of integers

/**************************** module_param_cb ***************************************************/

int notify_param(const char *val, const struct kernel_param *kp) {
	int res = param_set_int(val, kp);		//use helper for write variable
	if(res == 0) {
		printk(KERN_ALERT "Callback function called ... \n");
		printk(KERN_ALERT "New Value of cb_values is :: %d\n");
		return 0;
	}
	return -1;
}

const struct kernel_param_ops my_param_ops = 
{
	.set = &notify_param, //use our setter ...
	.get = &param_get_int, //... and standard getter
}

module_param_cb(cb_value, &my_param_ops, &cb_value, S_IRUGO | S_IWUSR);

/**************************************************************************************************/

static int __init modpar_start(void) {
	int i = 0;
	printk(KERN_ALERT "charvar = %s\n", charvar);
	printk(KERN_ALERT "intvar =  %d\n", intvar);
	printk(KERN_ALERT "cb_value = %d\n", cb_value);

	for(i = 0; i < 3; i++) {
		printk(KERN_ALERT "arr_value[%d] = %d\n" , i, arr_value[i]);
	}
	printk(KERN_ALERT "Kernle Module Inserted Successfully\n");

	return 0;
}

static void __exit modpar_end(void) {

	printk(KERN_ALERT "We're going out. See you again!\n");
}

module_init(modpar_start);
module_exit(modpar_end);
