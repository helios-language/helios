#ifndef BUILTINS_H
#define BUILTINS_H

/**
 * This header autoincludes all different builtin helios datatypes.
 * It also defines self number of macros to use as shortcut to common operations
 * accepting any kind (so not type specific!) of helios object.
 */

#include <builtinutils.h>

#include <helios_boolean.h>
#include <helios_code.h>
#include <helios_float.h>
#include <helios_hashmap.h>
#include <helios_integer.h>
#include <helios_object.h>
#include <helios_string.h>

#include <helios_memory.h>
#include <serialize.h>

helios_object *helios_equal(helios_object *self, helios_object *other);
helios_object *helios_greater(helios_object *self, helios_object *other);
helios_object *helios_less(helios_object *self, helios_object *other);
helios_object *helios_notequal(helios_object *self, helios_object *other);
helios_object *helios_greaterequal(helios_object *self, helios_object *other);
helios_object *helios_lessequal(helios_object *self, helios_object *other);
helios_object *helios_hash(helios_object *obj);

#endif