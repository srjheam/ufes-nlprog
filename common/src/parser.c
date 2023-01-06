#include <stdlib.h>
#include <errno.h>

#include "parser.h"

bool tryParseInt(const char *str, int *val) {
    errno = 0;

    char *endptr;
    *val = strtol(str, &endptr, 10);

    if (errno != 0 || endptr == str)
        return false;

    return true;
}

bool tryParseFloat(const char *str, float *val) {
    errno = 0;

    char *endptr;
    *val = strtof(str, &endptr);

    if (errno != 0 || endptr == str)
        return false;

    return true;
}
