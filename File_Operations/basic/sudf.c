
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/types.h>

static int __init driver_start(void);
static void __exit driver_exit(void);

static int 		su_open(struct inode *inode, struct file *file);
static int 		su_release(struct inode *inod, struct file *file);
static ssize_t	su_read(struct file *filp, char __user *buff, size_t len, loff_t *off);
static ssize_t	su_write(struct file *filp, const char __user *buff, size_t len, loff_t *off); 

dev_t dev = 0;
struct cdev dev_cdev;


static struct file_operations fops = 
{
    .owner      = THIS_MODULE,
    .open       = su_open,
    .release    = su_release,
	.read		= su_read,
	.write		= su_write,      
};

static int su_open(struct inode *inode, struct file *file)
{
	pr_info("Driver Open Function Called..\n");
	return 0;	
}

static int su_release(struct inode *inode, struct file *file)
{
	pr_info("Driver Release Function Called..\n");
	return 0;
}

static ssize_t su_read(struct file *filp, char __user *buff, size_t len, loff_t *off)
{
	printk("Driver Read Function Called..\n");
	return 0;
}

static ssize_t su_write(struct file *filp, const char __user *buff, size_t len, loff_t *off)
{
	printk("Driver Write Function Called..\n");
	return 0;
}

static int __init driver_start(void)
{
    /*	Allocating Major and Minor numbers	*/
	if((alloc_chrdev_region(&dev, 0, 1, "sudf_Dev")) < 0) {
		pr_err("Cannot allocate major number and minor number\n");
		return -1;
	}
	pr_info("Major = %d	Minor = %d", MAJOR(dev), MINOR(dev));
	pr_info("Major and Minor numbers allocated successfully\n");

	cdev_init(&dev_cdev, &fops);

    /*	Adding Character device to the system		*/
	if((cdev_add(&dev_cdev, dev, 1)) < 0) {
		pr_err("Cannot add character device to the system\n");
		unregister_chrdev_region(dev, 1);
	    return -1;
	}
    pr_info("Character Device added successfully\n");


    pr_info("device created successfully\n");
    pr_info("Device Driver Insert Successfully ... \n");
    return 0;
}

static void __exit driver_exit(void)
{
    cdev_del(&dev_cdev);
	unregister_chrdev_region(dev, 1);
	pr_info("Device Driver Removed Successfully...\n\n\n");
}

module_init(driver_start);
module_exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sahil Usmani");
