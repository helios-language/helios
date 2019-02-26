#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <error.h>

void error_print(Error_t err) {
    printf("%s in line %i character %i\n", err.message, err.line,
           err.character);
}

void error_throw(Error_t err, const char* code, bool hard) {
    if (code == NULL) {
        error_print(err);
        if (hard) {
            exit(-1);
        } else {
            return;
        }
    } else {
        error_print(err);
        char* line = strtok(strdup(code), "\n");
        for (uint32_t i = 0; i < err.line; i++) {
            line = strtok(NULL, "\n");
        }
        if (line != NULL && *line != '\0' && err.character < strlen(line)) {
            printf("%s\n", line);
            for (uint32_t i = 0; i < err.character; i++) {
                printf(" ");
            }
            printf("^\n");
        } else {
            printf(
                "line or character index out of bounds. could not display "
                "error line information\n");
        }
        if (hard) {
            exit(-1);
        }
    }
}

Errorstack_t* errorstack_new() {
    Errorstack_t* es = malloc(sizeof(Errorstack_t));
    *es = (Errorstack_t){0, ERRORSTACK_DEFAULT_SIZE,
                         malloc(ERRORSTACK_DEFAULT_SIZE * sizeof(Error_t))};
    return es;
}

void errorstack_free(Errorstack_t* es) {
    free(es->stack);
    free(es);
}

void errorstack_push(Errorstack_t* es,
                     const char* message,
                     uint32_t line,
                     uint32_t character) {
    es->stack[es->filled++] = (Error_t){line, character, message};
    if (es->filled >= es->size) {
        es->size <<= 1;
        es->stack = realloc(es->stack, es->size * sizeof(Error_t));
    }
}
uint32_t errorstack_length(Errorstack_t* es) {
    return es->filled;
}

Error_t errorstack_pop(Errorstack_t* es) {
    return es->stack[--es->filled];
}

void errorstack_popuntil(Errorstack_t* es, uint32_t length) {
    while (es->filled > length) {
        errorstack_pop(es);
    }
}

Error_t errorstack_top(Errorstack_t* es) {
    return es->stack[es->filled - 1];
}

bool errorstack_empty(Errorstack_t* es) {
    return es->filled == 0;
}

void errorstack_traceback(Errorstack_t* es, const char* code) {
    error_throw(errorstack_top(es), code, false);
    for (int32_t i = es->filled - 2; i >= 0; i--) {
        printf("due to: ");
        error_throw(es->stack[i], NULL, false);
    }
    exit(-1);
}
