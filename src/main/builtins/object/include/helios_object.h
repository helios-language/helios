#ifndef HELIOS_OBJECT_H

#define HELIOS_OBJECT_H

#include <helios_type.h>

/**
 * Definition of the most basic helios object. Cannot be initialized. Only used
 * as a common type which can be cast to.
 */
typedef struct __helios_object_s {
    HELIOS_OBJECT_COMMON_BASE;
} helios_object;

helios_type helios_object_type;

// casting

//! cast any generic helios object to a helios_object object.
#define TO_HELIOS_OBJECT(obj) ((struct __helios_object_s *)obj)
//! return 1/true (c int) if obj is an instance of helios_object. Note:
//! subclasses aren't covered.
#define IS_HELIOS_OBJECT(obj) (obj->class == &helios_integer_type)

#endif
