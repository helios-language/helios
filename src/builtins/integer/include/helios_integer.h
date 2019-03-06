
#ifndef HELIOS_INTEGER_H
#define HELIOS_INTEGER_H

#include <helios_object.h>

#define HELIOS_INTEGER_DEFAULT_VALUE 0

typedef struct __helios_integer_s {
    HELIOS_OBJECT_COMMON_BASE;
    int32_t value;
} helios_integer;

helios_object helios_integer_type;

#define TO_HELIOS_INTEGER(obj) ((struct __helios_integer_s *)obj)
#define IS_HELIOS_INTEGER(obj) (obj->class == &helios_integer_type)

#endif
