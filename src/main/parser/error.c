#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <parser_error.h>
/**
 *  prints an error struct
 *  @param err the error to print
 */
void error_print(parser_error err) {
    printf("%s in line %i\n", err.message, err.line);
}

/*
 * Throws an error. This includes printing it, showing line and character
 * information, pointing at the line and character that caused it and
 * possibly exiting.
 *
 * @param err the error to throw
 * @param code the code to use when pointing out the location of the error.
 * Note: optional. Ignored if NULL.
 * @param hard boolean if throwing this error should exit the compilation
 * process.
 */
void error_throw(parser_error err, const char *code, bool hard) {
    if (code == NULL) {
        error_print(err);
        if (hard) {
            exit(-1);
        } else {
            return;
        }
    } else {
        error_print(err);
        uint32_t codelength = strlen(code);
        char newcode[codelength + 1];
        for (uint32_t i = 0; i < codelength; i++) {
            newcode[i] = code[i];
        }
        newcode[codelength] = '\0';

        uint32_t start = 0;
        uint32_t end = 0;
        for (uint32_t i = 0; i < err.line - 1; i++) {
            while (newcode[start++] != '\n') {
                if (newcode[start] == '\0') {
                    printf("line or character index out of bounds. could not "
                           "display "
                           "error line information.\n");
                    exit(-1);
                }
            }
        }

        char *line = newcode + start;
        while (line[end++] != '\n') {
            if (newcode[start] == '\0') {
                printf(
                    "line or character index out of bounds. could not display "
                    "error line information.\n");
                exit(-1);
            }
        }
        line[end] = '\0';

        if (line != NULL && *line != '\0' && (err.character) < strlen(line)) {
            printf("%s\n", line);
            for (uint32_t i = 0; i < err.character; i++) {
                printf(" ");
            }
            printf("^\n");

            for (uint32_t i = 0; i < strlen(line); i++) {
                printf("%i,", line[i]);
            }
            printf("\n");
        } else {
            printf("line or character index out of bounds. could not display "
                   "error line information.\n");
        }
        if (hard) {
            exit(-1);
        }
    }
}

/**
 * Creates a new errorstack object. Initializes it with some default values
 * specified in error.h.
 *
 * @return the newly created errorstack.
 */
errorstack *errorstack_new() {
    errorstack *es = malloc(sizeof(errorstack));
    *es = (errorstack){0, ERRORSTACK_DEFAULT_SIZE,
                       malloc(ERRORSTACK_DEFAULT_SIZE * sizeof(parser_error))};
    return es;
}

/*
 * Frees an errorstack.
 * @param es the errorstack to free.
 */
void errorstack_free(errorstack *es) {
    free(es->stack);
    free(es);
}

/**
 * Construct a new error object out of parameters, and push it to the
 * errorstack.
 * @param es the errorstack to push the error to.
 * @param message the message the error should display when thrown.
 * @param line the line number of this error. NOTE: can easily be optained
 * with parser->line
 * @param character the character position of this error. NOTE: can easily be
 * optained with parser->character
 */
void errorstack_push(errorstack *es, const char *message, uint32_t line,
                     uint32_t character) {
    es->stack[es->filled++] = (parser_error){line, character, message};
    if (es->filled >= es->size) {
        es->size <<= 1;
        es->stack = realloc(es->stack, es->size * sizeof(parser_error));
    }
}

/**
 * Find the length of an errorstack.
 * @param es the errorstack to find the length of
 * @return the length of the stack.
 */
uint32_t errorstack_length(errorstack *es) {
    return es->filled;
}

/**
 * Pop and return the top element of the errorstack. For internal use only
 * and not exported by error.h.
 *
 * @param es the errorstack to pop
 * @return the top of the errorstack.
 */
parser_error errorstack_pop(errorstack *es) {
    return es->stack[--es->filled];
}

/**
 * pop the errorstack until it has a desired length. Useful for popping the
 * errorstack back to a certain state recorded with errorstack_length. Much
 * more efficient than copying the entire stack.
 * @param es the errorstack to pop.
 * @param length the desired length. NOTE: will return immediately when the
 * length is already reached or the length is lower than the desired
 * length.
 */
void errorstack_popuntil(errorstack *es, uint32_t length) {
    while (es->filled > length) {
        errorstack_pop(es);
    }
}

/**
 * Return the top error of the errorstack.
 * @param es the errorstack to find the top of.
 * @return the top element of the errorstack.
 */
parser_error errorstack_top(errorstack *es) {
    return es->stack[es->filled - 1];
}

/**
 * Return true if a given errorstack is empty.
 * @param es the errorstack to check.
 * @return a boolean if the errorstack is empty.
 */
bool errorstack_empty(errorstack *es) {
    return es->filled == 0;
}

/**
 * Throw all errors on the errorstack. Show line numbering and location
 * with the top element on the stack and exit after.
 * @param es the errorstack to traceback
 * @param code the code to display when throwing errors. NOTE: ignored when
 * NULL.
 */
void errorstack_traceback(errorstack *es, const char *code) {
    error_throw(errorstack_top(es), code, false);
    for (int32_t i = es->filled - 2; i >= 0; i--) {
        printf("due to: ");
        error_throw(es->stack[i], NULL, false);
    }
    exit(-1);
}
