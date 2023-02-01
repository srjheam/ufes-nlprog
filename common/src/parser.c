#include <errno.h>
#include <limits.h>
#include <stdlib.h>

#include "exception.h"

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

int parseInt(const char *str) {
    // Check
    // - https://stackoverflow.com/a/9748431/12511877

    char *endptr;

    errno = 0;
    long conv = strtol(str, &endptr, 10);

    if (errno != 0 || *endptr != '\0' || conv > INT_MAX || conv < INT_MIN) {
        exception_throw("ArgumentException",
                        "Invalid integer string at - parser.parseInt",
                        EXIT_FAILURE);
    }

    return conv;
}
