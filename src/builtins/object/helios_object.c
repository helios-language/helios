
#include <helios_object.h>
#include <helios_type.h>

/**
 * The type of the most basic helios object. Only used for common casts. Cannot
 * be instantiated.
 */
helios_type helios_object_type = {
    HELIOS_OBJECT_COMMON_BASE_INIT(&helios_object_type),
};