#include <builtins.h>
#include <executestack.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
 */
void execstack_delete(execstack *stack) {
    helios_garbage_collect(stack->gc);
    free(stack->stack);
    free(stack);
}

void execstack_push(execstack *stack, helios_object *obj) {
    stack->stack[stack->filled++] = obj;
    if (stack->filled >= stack->size) {
        stack->size <<= 1;
        stack->stack =
            realloc(stack->stack, stack->size * sizeof(helios_object *));
    }
}

helios_object *execstack_pop(execstack *stack) {
    if (stack->filled == 0) {
        printf("execute stack empty, cannot pop.");
        exit(-1);
    }
    helios_object *res = stack->stack[--stack->filled];
    return res;
}

helios_object *execstack_top(execstack *stack) {
    if (stack->filled == 0) {
        printf("execute stack empty, cannot pop.");
        exit(-1);
    }
    return stack->stack[stack->filled - 1];
}

helios_object *execstack_topn(execstack *stack, uint32_t index) {
    if (index > (stack->filled - 1)) {
        printf("execute stack empty, cannot pop.");
        exit(-1);
    }
    return stack->stack[stack->filled - (index + 1)];
}

bool execstack_empty(execstack *stack) {
    return stack->filled == 0;
}

void execstack_clear(execstack *stack) {
    while (stack->filled > 0) {
        execstack_pop(stack);
    }
}

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