/*
 * @file 	cpyrdwr.c
 *
 * @Author 	Sahil Usmani
 *
 * @Brief	Simple program to understand copy to and from user
 *
 * @Date	20-Jan-2023	
 *
 * @Version	1.0
 * */


#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#define MEM_SIZE 1024
uint8_t *kernel_buffer;
uint8_t *user_buff;

static int 	__init driver_start(void);
static void 	__exit driver_stop(void);

static int	su_open(struct inode *inode, struct file *file);
static int	su_release(struct inode *inode, struct file *file);
static ssize_t	su_read(struct file *filp, char __user *buff, size_t len, loff_t *off);
static ssize_t	su_write(struct file *filp, const char __user *buff, size_t len, loff_t *off);

struct file_operations fops =
{
	.owner		= THIS_MODULE,
	.open		= su_open,
	.release	= su_release,
	.read		= su_read,
	.write		= su_write,
};

dev_t dev;				// for major and minor numbers
static struct class *dev_class;		// for return value from class_create (struct pointer)
static struct cdev su_cdev;		// for registering the cdev 

static int su_open(struct inode *inode, struct file *file) 
{
	printk("Open Fucntion is called..\n");
	return 0;
}

static int su_release(struct inode *inode, struct file *file) 
{
	printk("Release Function is called..\n");
	return 0;
}

static ssize_t su_read(struct file *filp, char __user *buff, size_t len, loff_t *off)
{
	printk("Read Function is called..\n");
	if(copy_to_user(user_buff, kernel_buffer, MEM_SIZE))
	{
		printk(KERN_ALERT "Data read error\n");
	}
	printk("Data read done");
	return MEM_SIZE;
}

static ssize_t su_write(struct file *filp, const char __user *buff, size_t len, loff_t *off) 
{
	printk("Write Function is called..\n");
	if(copy_from_user(kernel_buffer, user_buff, len))
	{
		printk(KERN_ALERT "Data write error\n");
	}
	printk("Data write done\n");
	return len;
}

static int __init driver_start(void) 
{

	printk(KERN_ALERT "Inserting Device Driver into Kernel\n");

	if((alloc_chrdev_region(&dev, 0, 1, "dev_region")) < 0)
	{
		printk("Cannot allocate region\n");
		return -1;
	}
	printk("Major = %d	Minor = %d\n", MAJOR(dev), MINOR(dev));

	cdev_init(&su_cdev, &fops);

	if((cdev_add(&su_cdev, dev, 1) < 0))
	{
		printk("Cannot add device to the systme\n");
		unregister_chrdev_region(dev, 1);
		return -1;
	}

	if((dev_class = class_create(THIS_MODULE, "su_class")) == NULL)
	{
		printk("Cannot create class\n");
		unregister_chrdev_region(dev, 1);
		return -1;
	}

	if((device_create(dev_class, NULL, dev, NULL, "cptfuser_device")) == NULL)
	{
		printk("Cannot create the device\n");
		class_destroy(dev_class);
	}

	/* Creating Physical Memory in Kernel */
	if((kernel_buffer = kmalloc(MEM_SIZE, GFP_KERNEL)) == 0)
	{
		printk("Cannot allocate memory in kernel\n");
		class_destroy(dev_class);
	}

	pr_alert("Device Driver inserted successfully\n");
	return 0;
}

static void __exit driver_stop(void) 
{
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	cdev_del(&dev);
	unregister_chrdev_region(dev, 1);
	printk(KERN_ALERT "Device Driver removed successfully\n\n\n");
}

module_init(driver_start);
module_exit(driver_stop);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sahil Usmani");
MODULE_VERSION("1.0");
