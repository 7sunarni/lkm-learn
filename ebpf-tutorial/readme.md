# ebpf-turorial

https://github.com/xdp-project/xdp-tutorial

## check log
```shell
cat /sys/kernel/debug/tracing/trace_pipe
```

# remove xdp prog
ip link set veth1 xdpgeneric off
