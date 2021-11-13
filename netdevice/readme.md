# ovs 针对不同的模式有不同的处理方式

stt(Stateless TCP Tunnel) 是通过 netfilter 挂载的方式
vxlan, geneve 模式是通过新的虚拟网络设备来实现的。

# demo 虚拟网络设备的实现。
Origin: [https://github.com/g0dA/linuxStack/blob/master/linux%E7%BD%91%E7%BB%9C(%E4%B8%89).md](https://github.com/g0dA/linuxStack/blob/master/linux%E7%BD%91%E7%BB%9C(%E4%B8%89).md)

```sh
# compile
make all
# install
sudo insmod demo_nd.ko
# view
sudo ip link show demo-nd
# enable
sudo ip link set demo-nd up
# disable 
sudo ip link set demo-nd down
# uninstall
sudo rmmod demo_nd
# viewlog
sudo cat /var/logs/messages | grep "===" # or sudo dmesg | grep "==="
```

## 数据处理
当有数据包来的时候，会调用 ndo_start_xmit 的函数处理。
