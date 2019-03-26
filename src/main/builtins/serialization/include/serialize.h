
#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <helios_type.h>

#define HELIOS_SERIALIZED_OBJECT_DEFAULT_SIZE 10

typedef struct __helios_serialized_object_s {
    HELIOS_OBJECT_COMMON_BASE;
    uint32_t filled;
    uint32_t size;
    uint8_t *data;
} helios_serialized_object;

helios_type helios_serialized_object_type;

helios_object *helios_serialized_object_init();
helios_object *helios_serialize_object(helios_object *obj);
helios_object *helios_deserialize_object(helios_object *self);
void helios_serialized_object_delete(helios_object *self);
void helios_serialized_object_print(helios_object *self);

//! cast any generic helios object to a helios_serialized_object object.
#define TO_HELIOS_SERIALIZED_OBJECT(obj)                                       \
    ((struct __helios_serialized_object_s *)obj)

//! return 1/true (c int) if obj is an instance of helios_serialized_object.
//! Note: subclasses aren't covered.
#define IS_HELIOS_SERIALIZED_OBJECT(obj)                                       \
    (obj->class == &helios_serialized_object_type)

#endif