#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <error.h>

void error_print(Error_t err) {
    /*
        prints an error struct
        @param err the error to print
    */

    printf("%s in line %i character %i\n", err.message, err.line,
           err.character);
}

void error_throw(Error_t err, const char* code, bool hard) {
    /*
        Throws an error. This includes printing it, showing line and character
        information, pointing at the line and character that caused it and
        possibly exiting.

        @param err the error to throw
        @param code the code to use when pointing out the location of the error.
        Note: optional. Ignored if NULL.
        @param hard boolean if throwing this error should exit the compilation
        process.
     */
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
    /*
        Creates a new errorstack object. Initializes it with some default values
        specified in error.h.

        @return the newly created errorstack.
     */
    Errorstack_t* es = malloc(sizeof(Errorstack_t));
    *es = (Errorstack_t){0, ERRORSTACK_DEFAULT_SIZE,
                         malloc(ERRORSTACK_DEFAULT_SIZE * sizeof(Error_t))};
    return es;
}

void errorstack_free(Errorstack_t* es) {
    /*
        Frees an errorstack.
        @param es the errorstack to free.
    */
    free(es->stack);
    free(es);
}

void errorstack_push(Errorstack_t* es,
                     const char* message,
                     uint32_t line,
                     uint32_t character) {
    /*
        Construct a new error object out of parameters, and push it to the
        errorstack.
        @param es the errorstack to push the error to.
        @param message the message the error should display when thrown.
        @param line the line number of this error. NOTE: can easily be optained
        with parser->line
        @param line the character position of this error. NOTE: can easily be
        optained with parser->character
    */
    es->stack[es->filled++] = (Error_t){line, character, message};
    if (es->filled >= es->size) {
        es->size <<= 1;
        es->stack = realloc(es->stack, es->size * sizeof(Error_t));
    }
}
uint32_t errorstack_length(Errorstack_t* es) {
    /*
        Find the length of an errorstack.
        @param es the errorstack to find the length of
        @return the length of the stack.
    */
    return es->filled;
}

Error_t errorstack_pop(Errorstack_t* es) {
    /*
        Pop and return the top element of the errorstack. For internal use only
        and not exported by error.h.

        @param es the errorstack to pop
        @return the top of the errorstack.
    */
    return es->stack[--es->filled];
}

void errorstack_popuntil(Errorstack_t* es, uint32_t length) {
    /*
        pop the errorstack until it has a desired length. Useful for popping the
        errorstack back to a certain state recorded with errorstack_length. Much
        more efficient than copying the entire stack.
        @param es the errorstack to pop.
        @param length the desired length. NOTE: will return immediately when the
        length is already reached or the length is lower than the desired
        length.
    */
    while (es->filled > length) {
        errorstack_pop(es);
    }
}

Error_t errorstack_top(Errorstack_t* es) {
    /*
        Return the top error of the errorstack.
        @param es the errorstack to find the top of.
        @return the top element of the errorstack.
    */
    return es->stack[es->filled - 1];
}

bool errorstack_empty(Errorstack_t* es) {
    /*
        Return true if a given errorstack is empty.
        @param es the errorstack to check.
        @return a boolean if the errorstack is empty.
    */
    return es->filled == 0;
}

void errorstack_traceback(Errorstack_t* es, const char* code) {
    /*
        Throw all errors on the errorstack. Show line numbering and location
        with the top element on the stack and exit after.
        @param es the errorstack to traceback
        @param code the code to display when throwing errors. NOTE: ignored when
        NULL.
    */
    error_throw(errorstack_top(es), code, false);
    for (int32_t i = es->filled - 2; i >= 0; i--) {
        printf("due to: ");
        error_throw(es->stack[i], NULL, false);
    }
    exit(-1);
}
