#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Anyone can use this test code");

int jonah_module_init(void) {
	int * num;
	printk(KERN_ALERT "Print num %d\n", *num);
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	return 0;
}

void jonah_module_exit(void) {
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
}

module_init(jonah_module_init);
module_exit(jonah_module_exit);
