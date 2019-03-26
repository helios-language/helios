
#ifndef EXECUTESTACK_H
#define EXECUTESTACK_H

#define EXECUTESTACK_DEFAULT_SIZE                                              \
    1024 // deliberately high so reallocation wont happen much

//! If the execstack has grown over its startsize (1024) but drops below this
//! percentage, the stack will be halved again. CANT BE OVER 0.5.
#define EXECUTESTACK_DEFAULT_FREE_PERCENT 0.3

#include <builtins.h>
#include <stdint.h>

typedef struct {
    uint32_t filled;
    uint32_t size;
    helios_object **stack;
    garbagecollector
        *gc; //!< Every execstack is tied to a garbage collector.
             //!< Later this will become a reference to a thread
             //!< object which holds the GC reference, but that's not in yet.
} execstack;

execstack *execstack_init();
void execstack_delete(execstack *stack);

void execstack_push(execstack *stack, helios_object *obj);
helios_object *execstack_pop(execstack *stack);
helios_object *execstack_top(execstack *stack);
helios_object *execstack_topn(execstack *stack, uint32_t index);
bool execstack_empty(execstack *stack);
void execstack_clear(execstack *stack);
void execstack_print(execstack *stack);

#endif
