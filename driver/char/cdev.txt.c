//手动创建字符设备节点
mknod -m 0755 /dev/test_ch c major minor


//注册字符设备
/*
* If @major == 0 this functions will dynamically allocate a major and return
* its number.
*
* 注册字符设备主要做了三个事情
* 1.register_chrdev_region
* 2.cdev_alloc
* 3.cdev_add
* 
* 注册字符设备的方式有两种:
* 1.调用register_chrdev()
* 2.
* 	register_chrdev_region()
* 	cdev_alloc()
* 	cdev_init()
* 	cdev_add()
*/
static inline int register_chrdev(unsigned int major, const char *name,
				  		const struct file_operations *fops)


static inline void unregister_chrdev(unsigned int major, const char *name)




//字符设备驱动模板
#define DEV_NAME "frank_ch"

int ch_open(struct inode *node, struct file * filp)
{
	return 0;
}


int ch_close(struct inode *node, struct file *filp)
{
	return 0;	
}

ssize_t ch_read(struct file *filp, char __user *data, size_t size, loff_t *off)
{
	return size;	
}

ssize_t ch_write(struct file *filp, const char __user *data, size_t size, loff_t *off)
{
	return size;	
}

int major = 0;//自动分配
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
	printk("chdev init\n");
	ret = register_chrdev(major, DEV_NAME, &fops);
	if (ret < 0)
		return ret;
	else
	{
		major = ret;	
		printk("major=%d\n", major);
	}

	return 0;
}


void ch_exit(void)
{
	printk("chdev exit\n");
	unregister_chrdev(major, DEV_NAME);
}

module_init(ch_init);
module_exit(ch_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("frank");
MODULE_VERSION("1.0");

