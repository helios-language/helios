
#ifndef HELIOS_HASHMAP_H
#define HELIOS_HASHMAP_H

#include <helios_object.h>
#include <stdbool.h>

#define HELIOS_HASHMAP_DEFAULT_SIZE 10
//! This is a feature that consumes 8 bytes more memory per stored hashmap item.
//! However, rehashing of hashmaps is considerably faster.
#define STORE_HASH_OPTIMIZATION true

#define HELIOS_HASHMAP_PERTURB_SHIFT 5

#define HELIOS_HASH_NEXT(curr, mod, perturb)                                   \
    (((curr << 2) + curr + perturb + 1) % mod)

//! size * (2/3) > filled
#define HELIOS_HASHMAP_TOO_FULL_FUNC(size) ((size << 1) / 3)
//! Accepts a size and %filled and returns if the dict should be rehashed up.
#define HELIOS_HASHMAP_TOO_FULL(size, filled)                                  \
    (filled > HELIOS_HASHMAP_TOO_FULL_FUNC(size))

//! TODO: not sure if this feature should be on
//! size * (1/3) > filled
#define HELIOS_HASHMAP_TOO_EMPTY_FUNC(size) ((size << 1) / 3)
//! TODO: not sure if this feature should be on
//! Accepts a size and %filled and returns if the dict should be rehashed down.
#define HELIOS_HASHMAP_TOO_EMPTY(size, filled) false

typedef struct {
#if STORE_HASH_OPTIMIZATION
    int64_t cachedhash;
#endif

    helios_object *key;
    helios_object *value;

    bool defunct;
} hashmap_entry;

typedef struct __helios_hashmap_s {
    HELIOS_OBJECT_COMMON_BASE;

    uint32_t filled;
    uint32_t size;
    hashmap_entry **value;
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

void helios_hashmap_put(helios_object *self, helios_object *key,
                        helios_object *value);
void helios_hashmap_remove(helios_object *self, helios_object *key);
helios_object *helios_hashmap_get(helios_object *self, helios_object *key);

#endif
