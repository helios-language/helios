#ifndef HELIOS_OBJECT_H

#define HELIOS_OBJECT_H

#include <helios_type.h>

typedef struct __helios_object_s {
    HELIOS_OBJECT_COMMON_BASE;
} helios_object;

helios_type helios_object_type;

// casting

#define TO_HELIOS_OBJECT(obj) ((struct __helios_object_s *)obj)
#define IS_HELIOS_OBJECT(obj) ((struct __helios_object_s *)obj)
#define CALL_HELIOS_OBJECT(func, obj, ...)                                     \
    (TO_HELIOS_OBJECT(obj)->class->func(obj, ##__VA_ARGS__))

#endif
