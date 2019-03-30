
#ifndef HELIOS_BOOLEAN_H
#define HELIOS_BOOLEAN_H

#include <helios_object.h>
#include <stdbool.h>

#define HELIOS_BOOLEAN_DEFAULT_VALUE helios_boolean_false

helios_object *helios_boolean_true;
helios_object *helios_boolean_false;

typedef struct __helios_boolean_s {
    HELIOS_OBJECT_COMMON_BASE;
    bool value;
} helios_boolean;

helios_type helios_boolean_type;

//! cast any generic helios object to a helios_boolean object.
#define TO_HELIOS_BOOLEAN(obj) ((struct __helios_boolean_s *)obj)
//! return 1/true (c int) if obj is an instance of helios_boolean. Note:
//! subclasses aren't covered.
#define IS_HELIOS_BOOLEAN(obj) (obj->class == &helios_boolean_type)

//! true if the object is helios_boolean_true
#define HELIOS_BOOLEAN_IS_TRUE(obj) (obj == helios_boolean_true)

//! true if the object is helios_boolean_true
#define HELIOS_BOOLEAN_IS_FALSE(obj) (obj == helios_boolean_false)

// global export of boolean functions

void helios_boolean_delete(helios_object *self);
helios_object *helios_boolean_init();
helios_object *helios_boolean_copy(helios_object *obj);

helios_object *helios_boolean_tostring(helios_object *self);
helios_object *helios_boolean_represent(helios_object *self);

helios_object *helios_boolean_equal(helios_object *self, helios_object *other);
helios_object *helios_boolean_less(helios_object *self, helios_object *other);
helios_object *helios_boolean_greater(helios_object *self,
                                      helios_object *other);
helios_object *helios_boolean_notequal(helios_object *self,
                                       helios_object *other);
helios_object *helios_boolean_greaterequal(helios_object *self,
                                           helios_object *other);
helios_object *helios_boolean_lessequal(helios_object *self,
                                        helios_object *other);
helios_object *helios_boolean_boolean(helios_object *self);

helios_object *helios_boolean_integer(helios_object *self);
helios_object *helios_boolean_from_cint(int32_t value);
helios_object *helios_boolean_from_cbool(bool value);

#endif
