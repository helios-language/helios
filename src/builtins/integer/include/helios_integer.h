
#ifndef HELIOS_INTEGER_H
#define HELIOS_INTEGER_H

#include <helios_object.h>

#define HELIOS_INTEGER_DEFAULT_VALUE 0

typedef struct __helios_integer_s {
    HELIOS_OBJECT_COMMON_BASE;
    int32_t value;
} helios_integer;

helios_type helios_integer_type;

//! cast any generic helios object to a helios_integer object.
#define TO_HELIOS_INTEGER(obj) ((struct __helios_integer_s *)obj)
//! return 1/true (c int) if obj is an instance of helios_integer. Note:
//! subclasses aren't covered.
#define IS_HELIOS_INTEGER(obj) (obj->class == &helios_integer_type)

// global export of integer functions

helios_object *helios_integer_add(helios_object *self, helios_object *other);
helios_object *helios_integer_subtract(helios_object *self,
                                       helios_object *other);
helios_object *helios_integer_divide(helios_object *self, helios_object *other);
helios_object *helios_integer_multiply(helios_object *self,
                                       helios_object *other);

void helios_integer_delete(helios_object *self);
helios_object *helios_integer_init();

helios_object *helios_integer_tostring(helios_object *self);
helios_object *helios_integer_represent(helios_object *self);

helios_object *helios_integer_from_cint(int32_t value);

#endif
