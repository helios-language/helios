
#ifndef HELIOS_MEMORY_H
#define HELIOS_MEMORY_H

#include <builtinutils.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define GC_DEFAULT_LENGTH 10
#define GC_REHASH_PERCENT ((double)55)

struct __helios_object_s;

typedef struct {
    bool used;
    struct __helios_object_s *obj;
} __helios_gc_hashmap_node;

typedef struct {
    uint32_t size;
    uint32_t filled;
    __helios_gc_hashmap_node *allocated_objects;
} garbagecollector;

garbagecollector *helios_get_garbagecollector();

void helios_set_garbagecollector(garbagecollector *gc);
garbagecollector *helios_init_garbagecollector();
void helios_delete_garbagecollector(garbagecollector *gc);
void helios_free_all_tracked(garbagecollector *gc);
void helios_garbage_collect(garbagecollector *gc);
struct __helios_object_s *helios_allocate_object(size_t size);
struct __helios_object_s *helios_allocate_object_on_gc(garbagecollector *gc,
                                                       size_t size);
void helios_free_object(struct __helios_object_s *obj);
void helios_set_garbagecollectable(struct __helios_object_s *obj);

/**
 *  the global current garbagecollector.
 */
garbagecollector *__GC_CURRENT;

/**
 * Get the garbagecollector of an object
 */
#define HELIOS_GET_OBJ_GC(obj) (((struct __helios_object_s *)obj)->gc)

/**
 * Increment the reference count of an object
 */
#define HELIOS_INCREF(obj) ((struct __helios_object_s *)obj->refcount++);
/**
 * Decrement the reference count of an object. When the count reaches 0 no
 * garbage collection will be performed.
 */
#define HELIOS_DECREF_NOGC(obj) ((struct __helios_object_s *)obj->refcount--);

/**
 * Decrement the reference count of an object. When the count reaches 0 (or
 * below) the object will be freed.
 */
#define HELIOS_DECREF(obj)                                                     \
    do {                                                                       \
        ((struct __helios_object_s *)obj)->refcount--;                         \
        if (((struct __helios_object_s *)obj)->refcount <= 0) {                \
            HELIOS_CALL_MEMBER(destructor, obj)                                \
        }                                                                      \
    } while (0);

/**
 * Sets the reference count of an object. Setting the reference count of an
 * object to 0 is equal to calling it's destructor.
 */
#define HELIOS_SETREF(obj, value)                                              \
    do {                                                                       \
        if (value == 0) {                                                      \
            HELIOS_CALL_MEMBER(destructor, obj)                                \
        } else {                                                               \
            ((struct __helios_object_s *)obj)->refcount = value;               \
        }                                                                      \
    } while (0);

/**
 * Get the reference count of an object.
 */
#define HELIOS_GETREF(obj) (((struct __helios_object_s *)obj)->refcount)

#endif