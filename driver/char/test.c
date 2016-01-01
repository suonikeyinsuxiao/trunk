#include <linux/module.h>
#include <linux/fs.h>

#define DEV_NAME "frank_ch"

/*
 *
 * If @major == 0 this functions will dynamically allocate a major and return
 * its number.
 *
 * If @major > 0 this function will attempt to reserve a device with the given
 * major number and will return zero on success.
static inline int register_chrdev(unsigned int major, const char *name,
				  const struct file_operations *fops)

static inline void unregister_chrdev(unsigned int major, const char *name)

	struct module *owner;
	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
	int (*open) (struct inode *, struct file *);
	int (*release) (struct inode *, struct file *);
*/

int ch_open(struct inode *node, struct file * filp)
{
	printk("ch open\n");
	printk("fops.owner->module_refptr->incs=%ld\n", filp->f_op->owner->refptr->incs);
	printk("fops.owner->module_refptr->desc=%ld\n", filp->f_op->owner->refptr->decs);

	return 0;
}

int ch_close(struct inode *node, struct file *filp)
{
	printk("ch close\n");
	printk("fops.owner->module_refptr->incs=%ld\n", filp->f_op->owner->refptr->incs);
	printk("fops.owner->module_refptr->desc=%ld\n", filp->f_op->owner->refptr->decs);
	return 0;
}

ssize_t ch_read(struct file *filp, char __user *data, size_t size, loff_t *off)
{
	printk("ch read\n");

	return size;
}

ssize_t ch_write(struct file *filp, const char __user *data, size_t size, loff_t *off)
{
	printk("ch write\n");

	return size;
}

int major = 0;
struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = ch_read,
	.write = ch_write,
	.open = ch_open,
	.release = ch_close,
};

int ch_init(void)
{
	int ret = 0;
	printk("test ch init\n");
	ret = register_chrdev(major, DEV_NAME, &fops);
	if (0 > ret)
		return ret;
	else
	{
		major = ret;	
		printk("major=%d\n", major);
	}

	printk("fops.owner->state=%d\n", fops.owner->state);
	printk("fops.owner->name=%s\n", fops.owner->name);
	printk("fops.owner->module_refptr->incs=%ld\n", fops.owner->refptr->incs);
	printk("fops.owner->module_refptr->desc=%ld\n", fops.owner->refptr->decs);

	return 0;
}

void ch_exit(void)
{
	printk("test ch exit\n");
	unregister_chrdev(major, DEV_NAME);
}

module_init(ch_init);
module_exit(ch_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("frank");
MODULE_VERSION("1.0");

