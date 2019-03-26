
#ifndef HELIOS_FLOAT_H
#define HELIOS_FLOAT_H

#include <helios_object.h>

#define HELIOS_FLOAT_DEFAULT_VALUE 0.0

typedef struct __helios_float_s {
    HELIOS_OBJECT_COMMON_BASE;
    double value;
} helios_float;

helios_type helios_float_type;

//! cast any generic helios object to a helios_float object.
#define TO_HELIOS_FLOAT(obj) ((struct __helios_float_s *)obj)
//! return 1/true (c int) if obj is an instance of helios_float. Note:
//! subclasses aren't covered.
#define IS_HELIOS_FLOAT(obj) (obj->class == &helios_float_type)

// global export of float functions

helios_object *helios_float_add(helios_object *self, helios_object *other);
helios_object *helios_float_subtract(helios_object *self, helios_object *other);
helios_object *helios_float_divide(helios_object *self, helios_object *other);
helios_object *helios_float_multiply(helios_object *self, helios_object *other);

void helios_float_delete(helios_object *self);
helios_object *helios_float_init();
helios_object *helios_float_copy(helios_object *obj);

helios_object *helios_float_tostring(helios_object *self);
helios_object *helios_float_represent(helios_object *self);

helios_object *helios_float_from_cdouble(double value);
helios_object *helios_float_from_cstring(char *value);

#endif
