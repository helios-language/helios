/**
 * Garbage collector objects are a field in any thread, and any object has a
 * pointer to one. A garbagecollector can be in one and only one thread. An
 * object can have only one garbagecollector and therefore an object can be in
 * only one thread. An object's refcount is only important for it's own garbage
 * collector.
 *
 * To be implemented: When an object is transfered between threads (which is
 * fully possible) it is (i am not entirely sure how this is going to work)
 * either completely copied or all references to it in the original thread are
 * broken (any subsequent access will result in an error). Any new object,
 * unless created with the special helios_allocate_object_on_gc function which
 * allocates an object on a specific garbage collector, will be allocated with
 * the garbagecollector of the current running thread. (currently a global
 * variable but this will be refactored to be inside thread objects).
 *
 * Warining: When running Helios code in c one has to always first create a
 * garbagecollector. When freeing a garbagecollector, all it's tracked objects
 * are untracked and freed too. It is however possible to free all objects in a
 * garbage collector without freeing it (for reuse) with
 * helios_free_all_tracked.
 *
 * TODO: maybe use the garbagecollector prefix more consistently and not at the
 * end of names like in helios_init_garbagecollector
 *
 * TODO: object reuse. instead of freeing it, keep it with a refcount of 0 in
 * the GC and allow it to be repurposed without calling malloc again.
 *
 * TODO: cycle detection
 */

#include <helios_memory.h>
#include <helios_object.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

garbagecollector *__GC_CURRENT = NULL;

/**
 * Get the global current garbage collector. Errors whenever this value is not
 * set.
 *
 * @return the current GC
 */
garbagecollector *helios_get_garbagecollector() {
    if (__GC_CURRENT == NULL) {
        printf(
            "ERROR: no garbage collector found (fix: use "
            "helios_init_garbagecollector and helios_set_garbagecollector)\n");
        exit(-1);
    }
    return __GC_CURRENT;
}

/**
 * Points the global garbagecollector pointer to a new garbagecollector.
 *
 * @param gc the new gc
 */
void helios_set_garbagecollector(garbagecollector *gc) {
    __GC_CURRENT = gc;
}

/**
 * Creates a new garbagecollector object. NOTE: doesnt automatically set it as
 * the current GC. This has to be done manually.
 *
 * TODO: create shortcut function for creating a GC and setting it as current.
 *
 * @return a newly created garbagecollector.
 */
garbagecollector *helios_init_garbagecollector() {
    garbagecollector *gc = malloc(sizeof(garbagecollector));
    *gc = (garbagecollector){
        GC_DEFAULT_LENGTH, 0,
        malloc(GC_DEFAULT_LENGTH * sizeof(__helios_gc_hashmap_node))};
    for (uint32_t i = 0; i < gc->size; i++) {
        gc->allocated_objects[i].used = false;
    }
    return gc;
}

/**
 * Free a garbagecollector object. Also frees all it's tracked objects.
 *
 * @param gc the garbagecollector to destroy.
 */
void helios_delete_garbagecollector(garbagecollector *gc) {
    helios_free_all_tracked(gc);
    free(gc->allocated_objects);
    free(gc);
}

/**
 *
 */
static uint64_t __hashfunction(uint64_t key) {
    key = (~key) + (key << 21);
    key = key ^ (key >> 24);
    key = (key + (key << 3)) + (key << 8);
    key = key ^ (key >> 14);
    key = (key + (key << 2)) + (key << 4);
    key = key ^ (key >> 28);
    key = key + (key << 31);
    return key;
}

/**
 * Checks the gc hashmap for the load percentage and rehashes the objects in
 * them when the number of objects exceeds the load percentage.
 *
 * @param gc the garbage collection to check
 */
static void rehash(garbagecollector *gc) {
    printf("%lf,%lf\n", ((double)gc->filled / (double)gc->size),
           (GC_REHASH_PERCENT / 100.0));
    if (((double)gc->filled / (double)gc->size) > (GC_REHASH_PERCENT / 100.0)) {
        __helios_gc_hashmap_node *newtrackedobjects =
            malloc(gc->size << 1 * sizeof(__helios_gc_hashmap_node));

        for (uint32_t i = 0; i < gc->size; i++) {
            gc->allocated_objects[i].used = false;
        }

        for (uint32_t i = 0; i < gc->size; i++) {
            if (gc->allocated_objects[i].used == true) {
                helios_object *obj = gc->allocated_objects[i].obj;

                uint64_t index =
                    __hashfunction((uint64_t)obj) % (gc->size << 1);

                while (newtrackedobjects[index].used == true) {
                    index = (index + 1) % gc->size;
                }

                gc->allocated_objects[index].used = true;
                gc->allocated_objects[index].obj = obj;
            }
        }

        free(gc->allocated_objects);
        gc->allocated_objects = newtrackedobjects;
        gc->size <<= 1;
    }
}

/**
 * Adds an object to the tracked_objects array of a GC. For internal use only.
 *
 * @param the object to start tracking.
 */
static void helios_track_object(helios_object *obj) {
    garbagecollector *gc = HELIOS_GET_OBJ_GC(obj);

    uint64_t index = __hashfunction((uint64_t)obj) % gc->size;

    while (gc->allocated_objects[index].used == true) {
        index = (index + 1) % gc->size;
    }

    gc->filled++;

    gc->allocated_objects[index].used = true;
    gc->allocated_objects[index].obj = obj;

    rehash(gc);
}

void helios_set_garbagecollectable(helios_object *obj) {
    obj->gc = helios_get_garbagecollector();
    helios_track_object(obj);
}

/**
 * Frees all tracked objects of a garbagecollector without freeing the GC
 * itself.
 *
 * @param gc the garbagecollector to free all tracked objects from
 */
void helios_free_all_tracked(garbagecollector *gc) {
    for (uint32_t i = 0; i < gc->filled; i++) {
        if (gc->allocated_objects[i].used == true) {
            HELIOS_CALL_MEMBER(destructor, gc->allocated_objects[i].obj);
        }
    }
}

/**
 * Perform a GC cleanup cycle. Iterates through a garbagecollector's tracked
 * objects and frees any object with a reference count which is less than or
 * equal to 0.
 *
 * @param gc the gc to perform garbage collection on
 */
void helios_garbage_collect(garbagecollector *gc) {
    for (uint32_t i = 0; i < gc->filled; i++) {
        if (gc->allocated_objects[i].used == true &&
            HELIOS_GETREF(gc->allocated_objects[i].obj) == 0) {
            HELIOS_CALL_MEMBER(destructor, gc->allocated_objects[i].obj);
        }
    }
}

/**
 * Allocates a new helios object (and performs all the GC steps associated with
 * that action). Use this instead of malloc whenever a helios object is created.
 *
 * NOTE: Automatically assigns the object's gc to be the current global GC
 * pointer. If this is not desired use helios_allocate_object_on_gc.
 *
 * @param size the size in bytes of an object to allocate
 * @return the newly allocated object
 */
helios_object *helios_allocate_object(size_t size) {
    helios_object *res = (helios_object *)malloc(size);
    return res;
}

/**
 * Allocates a new helios object (and performs all the GC steps associated with
 * that action). Use this instead of malloc whenever a helios object is created.
 * Sets the object's GC to whichever GC is passed in as the first argument.
 *
 * @param gc the garbagecollector to use for the allocated object.
 * @param size the size in bytes of an object to allocate
 * @return the newly allocated object
 */
helios_object *helios_allocate_object_on_gc(garbagecollector *gc, size_t size) {
    helios_object *res = (helios_object *)malloc(size);
    res->gc = gc;
    helios_track_object(res);
    return res;
}

/**
 * Frees a helios object. removes it from the it's GC's tracked_objects array.
 *
 * @param obj to object to free.
 */
void helios_free_object(helios_object *obj) {
    garbagecollector *gc = HELIOS_GET_OBJ_GC(obj);

    uint64_t index = __hashfunction((uint64_t)obj) % gc->size;

    while (gc->allocated_objects[index].used == true &&
           gc->allocated_objects[index].obj != obj) {
        index = (index + 1) % gc->size;
    }
    gc->allocated_objects[index].used = false;
    gc->filled--;
    free(obj);
}
