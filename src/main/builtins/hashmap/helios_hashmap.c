
#include <builtins.h>
#include <math.h>
#include <stdio.h>

/**
 * NOTE: for internal use only.
 * Allocates a new hashmap_entry object. Mostly for clarity.
 *
 * @return a new hashmap_entry object
 */
inline static hashmap_entry *hashmap_entry_new() {
    return malloc(sizeof(hashmap_entry));
}

/**
 * NOTE: for internal use only.
 *
 * Frees a hashmap_entry object. Mostly for clarity.
 * @param entry the entry to free
 */
inline static void hashmap_entry_free(hashmap_entry *entry) {
    free(entry);
}

/**
 * NOTE: for internal use only.
 * Rehashes a dictionary object to a new, larger size. Currently twice as large.
 *
 * @param self the object to rehash. it's values pointer will be replaced.
 */
static void helios_hashmap_rehash(helios_object *self) {
    helios_hashmap *hmself = TO_HELIOS_HASHMAP(self);
    // multiply size by 2
    uint32_t newsize = hmself->size << 1;

    hashmap_entry **newvalue = calloc(newsize, sizeof(hashmap_entry));

    for (uint32_t i = 0; i < hmself->size; i++) {
        if (hmself->value[i] == NULL || hmself->value[i]->defunct == true) {
            continue;
        }

#if STORE_HASH_OPTIMIZATION
        uint64_t hash = abs(hmself->value[i]->cachedhash);
#else
        uint64_t hash =
            helios_integer_to_cint(helios_hash(hmself->value[i].key));
#endif
        uint64_t newindex = hash % newsize;

        uint64_t perturb = hash;
        while (newvalue[newindex] != NULL &&
               newvalue[newindex]->defunct == false) {
            newindex = HELIOS_HASH_NEXT(newindex, newsize, perturb);
            perturb >>= HELIOS_HASHMAP_PERTURB_SHIFT;
        };

        newvalue[newindex] = hmself->value[i];
    }

    free(hmself->value);
    hmself->value = newvalue;
    hmself->size = newsize;
}

/**
 * Insert a key,value pair into a hashmap.
 *
 * @param self the hashmap to insert into
 * @param key the key of the pair
 * @param value the value of the pair
 */
void helios_hashmap_put(helios_object *self, helios_object *key,
                        helios_object *value) {
    helios_hashmap *hmself = TO_HELIOS_HASHMAP(self);
    uint32_t size = hmself->size;
    uint64_t hash = abs(helios_integer_to_cint(helios_hash(key)));
    int64_t index = hash % size;

    // search for this first non used value. so either defunct or null
    uint64_t perturb = hash;
    while (hmself->value[index] != NULL &&
           hmself->value[index]->defunct == false) {
        index = HELIOS_HASH_NEXT(index, size, perturb);
        perturb >>= HELIOS_HASHMAP_PERTURB_SHIFT;
    };

    // if not yet an entry, make one
    if (hmself->value[index] == NULL) {
        hmself->value[index] = hashmap_entry_new();
    }
    hmself->value[index]->defunct = false;
    hmself->value[index]->key = key;
    hmself->value[index]->value = value;
#if STORE_HASH_OPTIMIZATION
    hmself->value[index]->cachedhash = hash;
#endif

    HELIOS_INCREF(key);
    HELIOS_INCREF(value);

    if (HELIOS_HASHMAP_TOO_FULL(hmself->size, hmself->filled)) {
        helios_hashmap_rehash(self);
    }

    hmself->filled++;
}

void helios_hashmap_remove(helios_object *self, helios_object *key) {
}

helios_object *helios_hashmap_get(helios_object *self, helios_object *key) {
    // helios_hashmap *hmself = TO_HELIOS_HASHMAP(self);
    // uint32_t size = hmself->size;
    // uint64_t hash = abs(helios_integer_to_cint(helios_hash_object(key)));
    // uint64_t index = hash % size;

    // uint64_t perturb = hash;
    // while (hmself->value[index] != NULL) {
    //     if(hmself->value[index]){

    //     }

    //     index = helios_hash_next(index, size, perturb);
    //     perturb >>= HELIOS_HASHMAP_PERTURB_SHIFT;

    // };

    // printf("index not found \n");
    // exit(-1);
    return NULL;
}

void helios_hashmap_delete(helios_object *self) {
    for (uint32_t i = 0; i < TO_HELIOS_HASHMAP(self)->size; i++) {
        if (TO_HELIOS_HASHMAP(self)->value[i] == NULL) {
            continue;
        }
        if (TO_HELIOS_HASHMAP(self)->value[i]->defunct == false) {
            HELIOS_DECREF(TO_HELIOS_HASHMAP(self)->value[i]->key);
            HELIOS_DECREF(TO_HELIOS_HASHMAP(self)->value[i]->value);
        }
        hashmap_entry_free(TO_HELIOS_HASHMAP(self)->value[i]);
    }
    free(TO_HELIOS_HASHMAP(self)->value);
    helios_free_object(self);
}

helios_object *helios_hashmap_init() {
    helios_hashmap *self =
        (helios_hashmap *)helios_allocate_object(sizeof(helios_hashmap));
    *self = (helios_hashmap){
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_hashmap_type),
        .filled = 0,
        .size = HELIOS_HASHMAP_DEFAULT_SIZE,
        .value = calloc(HELIOS_HASHMAP_DEFAULT_SIZE, sizeof(hashmap_entry)),
    };

    helios_set_garbagecollectable(TO_HELIOS_OBJECT(self));

    return TO_HELIOS_OBJECT(self);
}

helios_object *helios_hashmap_tostring(helios_object *self) {
    return helios_string_from_charp("");
}

helios_object *helios_hashmap_represent(helios_object *self) {
    return helios_hashmap_tostring(self);
}

helios_object *helios_hashmap_copy(helios_object *self) {
    return NULL;
}

helios_type helios_hashmap_type = {
    HELIOS_OBJECT_COMMON_BASE_INIT(&helios_object_type),

    HELIOS_OBJECT_BASIC_FIELDS_CONSTRUCTOR(helios_hashmap_init),
    HELIOS_OBJECT_BASIC_FIELDS_DESTRUCTOR(helios_hashmap_delete),
    HELIOS_OBJECT_BASIC_FIELDS_CLASSNAME("Hashmap"),
    HELIOS_OBJECT_BASIC_FIELDS_REPRESENT(helios_hashmap_tostring),
    HELIOS_OBJECT_BASIC_FIELDS_TOSTRING(helios_hashmap_represent),
    HELIOS_OBJECT_BASIC_FIELDS_COPY(helios_hashmap_copy),

};
