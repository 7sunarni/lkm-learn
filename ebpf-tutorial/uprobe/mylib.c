#include <stdio.h>

/**

gcc -fPIC -shared -o libmylib.so mylib.c

 */
void hello_from_library(const char *name) {
    printf("Hello, %s! This is the shared library speaking.\n", name);
}
