#include <linux/linkage.h>
#include <linux/types.h>
#include <linux/psta.h>
#include <linux/kernel.h>

asmlinkage int sys_psta(struct pinfo *buf) {
	printk("Hello world\n");
	return 0;
}
