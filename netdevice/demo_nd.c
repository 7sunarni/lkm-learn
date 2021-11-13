#include <linux/module.h>
#include <linux/netdevice.h>

static struct net_device *demo_nd;

static int demo_nd_init(struct net_device *dev)
{
    printk("=== nd init");
    return 0;
};

static int demo_nd_open(struct net_device *dev)
{
    printk("=== nd open");
    return 0;
};

static int demo_nd_stop(struct net_device *dev)
{
    printk("=== nd stop");
    return 0;
};

static netdev_tx_t demo_nd_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
    printk("=== nd xmit");
    return NETDEV_TX_OK;
};

static int __init create_demo_nd(void)
{
    static struct net_device_ops ops = {
        .ndo_init = demo_nd_init,
        .ndo_start_xmit = demo_nd_start_xmit,
        .ndo_open = demo_nd_open,
        .ndo_stop = demo_nd_stop,

    };
    demo_nd = alloc_netdev(0, "demo-nd", NET_NAME_UNKNOWN, ether_setup);
    if (demo_nd == NULL)
    {
        return -ENOMEM;
    }
    demo_nd->netdev_ops = &ops;
    int err = register_netdevice(demo_nd);
    return err;
};

static void __exit delete_demo_nd(void)
{
    unregister_netdev(demo_nd);
    free_netdev(demo_nd);
};

module_init(create_demo_nd);
module_exit(delete_demo_nd);
