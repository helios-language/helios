
#include <helios_object.h>

helios_object helios_object_type = {
    HELIOS_OBJECT_COMMON_BASE_INIT(&helios_object_type),

    HELIOS_OBJECT_BASIC_FIELDS_NULL(),
    HELIOS_OBJECT_BASIC_FIELDS_CLASSNAME("Object"),

    HELIOS_OBJECT_BINOPS_FIELDS_NULL(),
};