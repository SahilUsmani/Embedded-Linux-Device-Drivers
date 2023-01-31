/*AUTOMATICALLY CREATING DEVICE FILE(udev)*/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>
 
dev_t dev = 0;
static struct class *dev_class;
 
/*
** Module init function
*/
static int __init char3_init(void)
{
        /*Allocating Major number*/
        alloc_chrdev_region(&dev, 0, 1, "char3_device");
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        /*Creating struct class*/
        dev_class = class_create(THIS_MODULE,"char3_class");             //ls -l /sys/class/char3_class/
        if(IS_ERR(dev_class)){
            pr_err("Cannot create the struct class for device\n");
            goto r_class;
        }
 
        /*Creating device*/
        if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"newnewnew"))){
            pr_err("Cannot create the Device\n");
            goto r_device;
        }
        pr_info("Kernel Module Inserted Successfully...\n");
        return 0;
 
r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}
 
/*
** Module exit function
*/
static void __exit char3_exit(void)
{
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        unregister_chrdev_region(dev, 1);
        pr_info("Kernel Module Removed Successfully...\n");
}
 
module_init(char3_init);
module_exit(char3_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("CDAC");
MODULE_DESCRIPTION("Automatically Creating a Device file");
