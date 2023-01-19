#include <stdio.h>
#include <stdlib.h>

#include "exception.h"

void exception_throw(char *name, char *msg, int code) {
    fprintf(stderr, "%s\n\n%s\n", name, msg);
    exit(code);
}

void exception_throw_OutOfMemory(char *msg) {
    exception_throw("OutOfMemoryException", msg, EXCEPTION_OUT_OF_MEMORY);
}
