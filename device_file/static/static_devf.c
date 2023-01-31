/*
 * @File	static_devf
 *
 * @Author	Sahil Usmani
 *
 * @Brief	A simple program demonstrating how a device file is created statically/manually
 *
 * @Version	1.0
 *
 * @Date	20-Jan-2023
 * 
 * */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>

dev_t dev;

static int __init driver_start(void)
{
	if((alloc_chrdev_region(&dev, 0, 1, "dev_region")) < 0)
	{
		printk("Cannot allocate major and minor numbers\n");
		return -1;
	}

	printk("Major = %d	Minor = %d\n", MAJOR(dev), MINOR(dev));
	printk("Device Driver inserted successfully\n\n");
	return 0;
}

static void __exit driver_stop(void)
{
	/*
	 * @Brief - unregistering the major and minor number / freeing the device number
	 * 	
	 * 	void unregister_chrdev_region(dev_t, unsigned int);
	 *
	 * 	dev_t 		- the variable where the numbers is stored
	 * 	unsigned int 	- the count of device files to be unregistered
	 * */
	unregister_chrdev_region(dev, 1);

	printk("Device Driver removed successfully\n\n\n");
}

module_init(driver_start);
module_exit(driver_stop);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sahil Usmani");
MODULE_VERSION("1.0");

/*
 * Check /proc/devices using " cat /proc/devices " you will find dev_region along with major number
 *
 * To create a device file :
 * step 1 :- insert the module using " sudo insmod <filename.ko> "
 * step 2 :- check the Major number and Minor number using " sudo dmesg "
 * step 3 :- now create the device file using utilty mknod
 * 		" sudo mknod -m <permissions> /dev/<device_file_name> c <major_number> <minor_number> "
 * */
