#include <stdio.h>

// Declare the function from the shared library
void hello_from_library(const char *name);

/*

gcc -o main main.c -L. -lmylib

*/
int main() {
    printf("Calling shared library function...\n");
    hello_from_library("Process 2");
    return 0;
}

/*

sudo bpftrace -e  'uprobe:./libmylib.so:hello_from_library { printf("%s\n", str(arg0)); }'

*/