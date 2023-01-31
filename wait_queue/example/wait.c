#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/wait.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/kthread.h>

uint32_t read_count = 0;
static struct task_struct *wait_thread;

DECLARE_WAIT_QUEUE_HEAD(wait_queue_hcwq);

dev_t dev = 0;
static struct class *dev_class;
static struct cdev hcwq_cdev;
int wait_queue_flag = 0;

/*
 *	Function Prototypes
 */
static int		__init hcwq_driver_start(void);
static void 	__exit hcwq_driver_end(void);




/******************************************** Driver Functions *******************************************************/
static int		hcwq_open(struct inode *inode, struct file *file);
static int		hcwq_release(struct inode *inode, struct file *file);
static ssize_t	hcwq_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t	hcwq_write(struct file *filp, const char *buf, size_t len, loff_t *off);


/*
 * 	File Operations Structures
 */
static struct file_operations fops =
{
	.owner		= THIS_MODULE,
	.read		= hcwq_read,
	.write		= hcwq_write,
	.open		= hcwq_open,
	.release	= hcwq_release,
};


/*
 * 	Thread Fucntion
 */
static int wait_function(void *unused)
{

	while(1)
	{
		pr_info("Waiting for event...\n");
		wait_event_interruptible(wait_queue_hcwq, wait_queue_flag != 0);
		if(wait_queue_flag == 2) 
		{
			pr_info("Event came from exit function...\n");
			return 0;
		}
		//  if(wait_queue_flag == 1)
        //         {
        //                 pr_info("Event came from read function...\n");
        //                 return 0;
        //         }

		pr_info("Event came from read function - %d\n", ++read_count);
		wait_queue_flag = 0;
	}
	do_exit(0);
	return 0;
}

/*
 *	This Function will be called when we open the device file
 */
static int hcwq_open(struct inode *inode, struct file *file)
{
	pr_info("Device file opened....\n");
	return 0;
}

/*
 *      This Function will be called when we close the device file
 */
static int hcwq_release(struct inode *inode, struct file *file)
{
        pr_info("Device file closed....\n");
        return 0;
}


/*
 *      This Function will be called when we read the device file
 */
static ssize_t hcwq_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        pr_info("Read Function....\n");
		wait_queue_flag = 1;
		wake_up_interruptible(&wait_queue_hcwq);
        return 0;
}



/*
 *      This Function will be called when we write to the device file
 */
static ssize_t hcwq_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        pr_info("Write Function....\n");
        return len;
}


	

static int __init hcwq_driver_start(void) 
{

	/* Allocating Major Number */
	if((alloc_chrdev_region(&dev, 0, 1, "hcwq_Dev")) < 0)
	{
		pr_info("Cannot allocate Major Number..\n");
		return -1;
	}

	pr_info("Major = %d\n Minor = %d\n", MAJOR(dev), MINOR(dev));

	/* Creating cdev structure */
	cdev_init(&hcwq_cdev, &fops);

	hcwq_cdev.owner = THIS_MODULE;
	hcwq_cdev.ops = &fops;

	/* Adding Char Device to the system */
	if((cdev_add(&hcwq_cdev, dev, 1)) < 0)
	{
		pr_info("Cannot add device to the system..\n");
		goto r_class;
	}

	/* Creating Struct class */
	if((dev_class = class_create(THIS_MODULE, "hcwq_class")) == NULL)
	{
		pr_info("Cannot create struct class ..\n");
		goto r_class;
	}

	/* Creating Device */
	if((device_create(dev_class, NULL, dev, NULL, "hcwq_device")) == NULL)
	{
		pr_info("Cannot create a device...\n");
		goto r_device;
	}

	/* Create a kernel thread with name mythread */
	wait_thread = kthread_create(wait_function, NULL, "WaitThread");
	if(wait_thread)
	{
		pr_info("Thread Created Successfully...\n");
		wake_up_process(wait_thread);
	}
	else
	{
		pr_info("Thread creation failed..\n");
	}

	pr_info("Device Driver Insert ..... Done\n");
	return 0;	

r_device:
    	class_destroy(dev_class);

r_class:
    	unregister_chrdev_region(dev,1);
        return -1;
}


static void __exit hcwq_driver_end(void) {

    wait_queue_flag = 2;
	wake_up_interruptible(&wait_queue_hcwq);
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	cdev_del(&hcwq_cdev);
	unregister_chrdev_region(dev, 1);
	pr_info("Device Driver Remove...Done\n");
}

module_init(hcwq_driver_start);
module_exit(hcwq_driver_end);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sahil Usmani");
