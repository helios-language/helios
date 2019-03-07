#ifndef BUILTINS_H
#define BUILTINS_H

/**
 * This header autoincludes all different builtin helios datatypes.
 * It also defines a number of macros to use as shortcut to common operations
 * accepting any kind (so not type specific!) of helios object.
 */

#include <helios_integer.h>
#include <helios_object.h>
#include <helios_string.h>

//! generic function for calling a member function of a helios_object struct.
//! note: doesn't include obj.__dict__ like lookup
#define HELIOS_CALL_MEMBER(func, obj, ...)                                     \
    (obj->class->func(obj, ##__VA_ARGS__))

#endif