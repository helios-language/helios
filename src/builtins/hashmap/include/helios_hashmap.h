
#ifndef HELIOS_HASHMAP_H
#define HELIOS_HASHMAP_H

#include <helios_object.h>
#include <stdbool.h>

typedef struct {
    helios_object *key;
    helios_object *value;

    bool defunct; // true when defunct
} hashmap_entry;

typedef struct __helios_hashmap_s {
    HELIOS_OBJECT_COMMON_BASE;

    uint32_t filled;
    uint32_t size;
    hashmap_entry contents[];
} helios_hashmap;

helios_type helios_hashmap_type;

//! cast any generic helios object to a helios_hashmap object.
#define TO_HELIOS_HASHMAP(obj) ((struct __helios_hashmap_s *)obj)
//! return 1/true (c int) if obj is an instance of helios_hashmap. Note:
//! subclasses aren't covered.
#define IS_HELIOS_HASHMAP(obj) (obj->class == &helios_hashmap_type)

// global export of hashmap functions

void helios_hashmap_delete(helios_object *self);
helios_object *helios_hashmap_init();

helios_object *helios_hashmap_tostring(helios_object *self);
helios_object *helios_hashmap_represent(helios_object *self);

// void helios_hashmap_put(helios_object *self, helios_object *obj);
// void helios_hashmap_remove(helios_object *self, helios_object *obj);
// helios_object *helios_hashmap_get(helios_object *self);

#endif
