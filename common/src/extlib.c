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
