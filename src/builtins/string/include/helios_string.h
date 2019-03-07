
#ifndef HELIOS_STRING_H
#define HELIOS_STRING_H

#include <helios_object.h>

#define HELIOS_STRING_DEFAULT_SIZE 10

typedef struct __helios_string_s {
    HELIOS_OBJECT_COMMON_BASE;
    uint32_t size;
    uint32_t filled;
    char *value;
} helios_string;

helios_type helios_string_type;

#define TO_HELIOS_STRING(obj) ((struct __helios_string_s *)obj)
#define IS_HELIOS_STRING(obj) (obj->class == &helios_string_type)
#define CALL_HELIOS_STRING(func, obj, ...)                                     \
    (TO_HELIOS_STRING(obj)->class->func(obj, ##__VA_ARGS__))

// global export of string functions

void helios_string_delete(helios_object *obj);
helios_object *helios_string_init();
helios_object *helios_string_from_cint(int32_t value);

#endif
