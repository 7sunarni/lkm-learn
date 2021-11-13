#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>

struct table_instance
{
	struct hlist_head *buckets;
};

static int __init LKM_init(void)
{
	struct table_instance *ti = kmalloc(sizeof(*ti), GFP_KERNEL);
	int table_size = 100;
	ti->buckets = kvmalloc_array(table_size, sizeof(struct hlist_head),
								 GFP_KERNEL);
	int i;
	for (i = 0; i < table_size; i++)
		INIT_HLIST_HEAD(&ti->buckets[i]);
	printk("%ll",sizeof(ti->buckets));
	return 0;
};

static void __exit LKM_exit(void)
{
	
};

module_init(LKM_init);
module_exit(LKM_exit);
