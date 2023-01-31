#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>

dev_t dev = 0;

static int __init start_alloc(void) {

	alloc_chrdev_region(&dev, 0, 2, "device_1");
	printk(KERN_ALERT "The chardev is allocated\n");
	printk(KERN_ALERT "The major number is %d\n", MAJOR(dev));
	printk(KERN_ALERT "The minor number is %d\n", MINOR(dev));
	return 0;
}

static void __exit end_alloc(void) {

	unregister_chrdev_region(dev, 1);
	printk(KERN_ALERT "The chardev is freed\n");
}

module_init(start_alloc);
module_exit(end_alloc);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sahil Usmani");
