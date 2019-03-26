#include <builtins.h>
#include <executestack.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Creates a new stack for executing bytecode on.
 *
 * @return the new stack
 */
execstack *execstack_init() {
    execstack *res = malloc(sizeof(execstack));
    *res = (execstack){
        0,
        EXECUTESTACK_DEFAULT_SIZE,
        malloc(EXECUTESTACK_DEFAULT_SIZE * sizeof(helios_object *)),
        helios_get_garbagecollector(),
    };
    return res;
}

/**
 * Deletes an execute stack. On deletion a stack is popped entirely, and a
 * garbagecolelction phase is executed. Therefore all objects which were only
 * referenced on this stack will be freed.
 *
 * @param stack the stack to remove
 */
void execstack_delete(execstack *stack) {
    helios_garbage_collect(stack->gc);
    free(stack->stack);
    free(stack);
}

/**
 * Pushes a helios_object onto the stack.
 *
 * @param stack the stack to push the object on
 * @param obj the object to push
 */
void execstack_push(execstack *stack, helios_object *obj) {
    stack->stack[stack->filled++] = obj;
    if (stack->filled >= stack->size) {
        stack->size <<= 1;
        stack->stack =
            realloc(stack->stack, stack->size * sizeof(helios_object *));
    }
}

/**
 * Pops the top of the executestack and returns it.
 *
 * @param stack the stack to pop
 * @return the object that used to be on top
 */
helios_object *execstack_pop(execstack *stack) {
    if (stack->filled == 0) {
        printf("execute stack empty, cannot pop.");
        exit(-1);
    }
    helios_object *res = stack->stack[--stack->filled];
    return res;
}

/**
 * Reads the top of the stack and returns it. Does not remove the top.
 *
 * @param stack the stack to read
 * @return the object that's on top of the stack
 */
helios_object *execstack_top(execstack *stack) {
    if (stack->filled == 0) {
        printf("execute stack empty, cannot pop.");
        exit(-1);
    }
    return stack->stack[stack->filled - 1];
}

/**
 * Read the nth item from the top and return it.
 *
 * @param stack the stack to read from
 * @param index the index you want from the top. 0 is equivalent to
 * execstack_top.
 * @return the object that was read
 */
helios_object *execstack_topn(execstack *stack, uint32_t index) {
    if (index > (stack->filled - 1)) {
        printf("execute stack empty, cannot pop.");
        exit(-1);
    }
    return stack->stack[stack->filled - (index + 1)];
}

/**
 * Returns true when the execstack is empty.
 *
 * @param stack the check to test for emptiness
 * @return empty or not
 */
bool execstack_empty(execstack *stack) {
    return stack->filled == 0;
}

/**
 * Pop the executestack until it's empty.
 *
 * @param stack the stack to clear
 */
void execstack_clear(execstack *stack) {
    while (stack->filled > 0) {
        execstack_pop(stack);
    }
}

/**
 * Print an execstack using the print functions of the objects on it.
 *
 * @param stack the stack to print
 */
void execstack_print(execstack *stack) {
    printf("execstack: [\n");
    for (uint32_t i = 0; i < stack->filled; i++) {
        helios_object *obj = stack->stack[i];
        if (HELIOS_HAS_MEMBER(tostring, obj)) {
            helios_object *string =
                HELIOS_CALL_MEMBER(tostring, stack->stack[i]);
            char *cstring = helios_string_to_charp(string);
            printf("%s\n", cstring);
            free(cstring);

        } else {
            printf("NULL\n");
        }
    }
    printf("]\n");
}