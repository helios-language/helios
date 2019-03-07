
#ifndef HELIOS_INTEGER_H
#define HELIOS_INTEGER_H

#include <helios_object.h>

#define HELIOS_INTEGER_DEFAULT_VALUE 0

typedef struct __helios_integer_s {
    HELIOS_OBJECT_COMMON_BASE;
    int32_t value;
} helios_integer;

helios_type helios_integer_type;

#define TO_HELIOS_INTEGER(obj) ((struct __helios_integer_s *)obj)
#define IS_HELIOS_INTEGER(obj) (obj->class == &helios_integer_type)
#define CALL_HELIOS_INTEGER(func, obj, ...)                                    \
    (TO_HELIOS_INTEGER(obj)->class->func(obj, ##__VA_ARGS__))

// global export of integer functions

helios_object *helios_integer_add(helios_object *self, helios_object *other);
helios_object *helios_integer_subtract(helios_object *self,
                                       helios_object *other);
helios_object *helios_integer_divide(helios_object *self, helios_object *other);
helios_object *helios_integer_multiply(helios_object *self,
                                       helios_object *other);

void helios_integer_delete(helios_object *obj);
helios_object *helios_integer_init();
helios_object *helios_integer_from_cint(int32_t value);

#endif
