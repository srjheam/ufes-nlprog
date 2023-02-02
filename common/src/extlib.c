#include <stdlib.h>

#include "exception.h"

#include "extlib.h"

int *intdup(const int *n) {
    int *dup = malloc(sizeof *dup);
    if (dup == NULL)
        exception_throw_OutOfMemory("intdup malloc failed");

    *dup = *n;

    return dup;
}

float *floatdup(const float *f) {
    float *dup = malloc(sizeof *dup);
    if (dup == NULL)
        exception_throw_OutOfMemory("intdup malloc failed");

    *dup = *f;

    return dup;
}

size_t hashStr(unsigned char *str) {
    // Check this out:
    // - https://stackoverflow.com/a/7666577/12511877

    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
