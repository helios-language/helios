
#include <ctype.h>
#include <errno.h>
#include <helios_float.h>
#include <helios_memory.h>
#include <helios_object.h>
#include <helios_string.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/**
 * Convert a string to a helios double.
 *
 * Link that helped a lot: http://krashan.ppa.pl/articles/stringtofloat/
 */
helios_object *helios_float_from_cstring(char *value) {
    bool sign = true;

    // skip leading spaces
    while (isspace(*value)) {
        ++value;
    }

    // read mantissa sign
    if (*value == '+') {
        value++;
    } else if (*value == '-') {
        value++;
        sign = false;
    }

    // shortcut on special values NaN and Infinity
    if (*value == 'N' && *(value + 1) != '\0' && *(value + 1) == 'a' &&
        *(value + 2) != '\0' && *(value + 2) == 'N') {
        return helios_float_from_cdouble(NAN);
    }
    if (*value == 'I' && *(value + 1) != '\0' && *(value + 1) == 'n' &&
        *(value + 2) != '\0' && *(value + 2) == 'f' && *(value + 3) != '\0' &&
        *(value + 3) == 'i' && *(value + 4) != '\0' && *(value + 4) == 'n' &&
        *(value + 5) != '\0' && *(value + 5) == 'i' && *(value + 6) != '\0' &&
        *(value + 6) == 't' && *(value + 7) != '\0' && *(value + 7) == 'y') {
        if (sign) {
            return helios_float_from_cdouble(INFINITY);
        } else {
            return helios_float_from_cdouble(-INFINITY);
        }
    }

    uint32_t length = strlen(value);
    char *valuecopy = malloc(length + 1);
    uint32_t i = 0;
    uint32_t j = 0;
    for (; j < length; i++, j++) {
        valuecopy[i] = value[j];
        if (value[j] == 'e') {
            if (value[j + 1] != '\0' && value[j + 1] == 'x' &&
                value[j + 2] != '\0' && value[j + 2] == 'p') {
                j += 2;
            }
        }
    }
    valuecopy[i] = '\0';

    char *end = NULL;
    double res = strtod(valuecopy, &end);

    if (res == 0.0 && (errno != 0 || end == valuecopy)) {
        printf("not a valid float\n");
        exit(-1);
    }

    while (*end != '\0') {
        if (!isspace(*end)) {
            printf("not a valid float\n");
            exit(-1);
        }
        end++;
    }

    free(valuecopy);

    if (sign) {
        return helios_float_from_cdouble(res);
    } else {
        return helios_float_from_cdouble(-res);
    }
}