# linux-kernel-module netfilter example

作用：类似 iptables 防火墙，丢掉访问 9090 端口的 udp 包

# 注册 netfiler 钩子
nf_register_net_hook

# 安装卸载
```shell
# install
sudo insmod drop_udp.ko
# check
sudo lsmod | grep drop_udp
# uninstall
sudo rmmod drop_udp
```

# 查看输出

```shell
sudo cat /var/log/messages 

Jul 29 10:17:43 zhanglixin-centos8 kernel: === lvm example: drop udp packet
```

# 参考
[https://infosecwriteups.com/linux-kernel-communication-part-1-netfilter-hooks-15c07a5a5c4e](https://infosecwriteups.com/linux-kernel-communication-part-1-netfilter-hooks-15c07a5a5c4e)
