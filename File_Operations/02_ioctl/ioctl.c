#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/device.h>

dev_t dev = 0;
static struct class *dev_class;
static struct cdev suioc_cdev;

/*Function Prototypes*/
static int      __init driver_init(void);
static void     __exit driver_exit(void);

static int      suioc_open(struct inode *inode, struct file *file);
static int      suioc_release(struct inode *inode, struct file *file);
static ssize_t  suioc_read(struct file *file, char __user *buff, size_t len, loff_t *off);
static ssize_t  suioc_write(struct file *file, const char __user *buff, size_t len, loff_t *off);
static int      suioc_ioctl(struct inode *inode, struct file *file, unsigned int, unsigned long)

struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .open       = suioc_open,
    .release    = suioc_release,
    .read       = suioc_read,
    .write      = suioc_write,            
};



static int __init driver_init(void) {
    
    if((alloc_chrdev_region(&dev, 0, 1, "suioc_Dev")) < 0) {
        pr_err("Cannot allocate major and minor numbers\n");
        return -1;
    }
    pr_info("Major = %d     Minor = %d\n", MAJOR(dev), MINOR(dev));
    
    cdev_init(&suioc_cdev, &fops);

    if((cdev_add(&suioc_cdev, dev, 1)) < 0) {
        pr_err("Device has not been added to the system\n");
        goto r_class;
    }


    if(IS_ERR(dev_class = class_create(THIS_MODULE, "suioc_class"))) {
        pr_err("Cannot create the struct class\n");
        goto r_class;
    }

    if(IS_ERR(device_create(dev_class, NULL, dev, NULL, "suioc_Device"))) {
        pr_err("Cannot create the Device 1\n");
        goto r_device;
    }
    return 0;

r_device:
    class_destroy(dev_class);

r_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}

static void __exit driver_exit(void) {
    
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&suioc_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("Driver is removed successfully\n\n\n");
}

module_init(driver_init);
module_exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sahil Usmani");
MODULE_DESCRIPTION("A simple module program to demonstrate IOCTL");
MODULE_VERSION("1.0");