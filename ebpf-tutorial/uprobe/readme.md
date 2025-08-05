# eBPF uprobe example

Simple example using eBPF uprobe tracing userspace dynamic shared library invoke

## 1. compile shared library
```bash
gcc -fPIC -shared -o libmylib.so mylib.c
```

## 2. compile binray
```bash
gcc -o main main.c -L. -lmylib
```

## 3. uprobe watch shared library
```bash
sudo bpftrace -e  'uprobe:./libmylib.so:hello_from_library { printf("hello_from_library invoke with parameter: %s\n", str(arg0)); }'
```

## 4. execute binary in another terminal
```bash
LD_LIBRARY_PATH=. ./main
```

And you will see some output in step 3 terminal

```bash
Attaching 1 probe...
hello_from_library invoke with parameter: Process 2
```

