
#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>
#include <stdint.h>

/**
 * As the errorstack uses an arraylist which dynamicly grows, this is the size
 * it always starts at. reallocation will make it dynamicly grow.
 */
#define ERRORSTACK_DEFAULT_SIZE 2

/**
 *  This struct represents an error object.
 */
typedef struct __error_s {
    uint32_t line;       //!< the line number of the occured error.
    uint32_t character;  //!< the character index of the occured error.
    const char *message; //!< the message associated with this error.
} parser_error;

/**
 * This struct represents a stack of errors. Whenever a new error is caused
 * its pushed to such a stack and then it may be ignored by the parser or
 * it's thrown. Whenever this happens the stack is walked down until its
 * empty and all errors on it are thrown, until the last one exits the
 * process. (this is the errorstack_traceback function in error.c)
 */
typedef struct __errorstack_s {
    uint32_t filled; //!< the amount of errors in the stack
    uint32_t size;   //!< the actual allocated size of the stack. Always larger
                     //!< than the filled parameter.
    parser_error *stack;
} errorstack;

void error_print(parser_error err);
void error_throw(parser_error err, const char *code, bool hard);

errorstack *errorstack_new();
void errorstack_free(errorstack *es);
void errorstack_push(errorstack *es, const char *message, uint32_t line,
                     uint32_t character);
uint32_t errorstack_length(errorstack *es);
parser_error errorstack_pop(errorstack *es);
void errorstack_popuntil(errorstack *es, uint32_t length);
parser_error errorstack_top(errorstack *es);
bool errorstack_empty(errorstack *es);
void errorstack_traceback(errorstack *es, const char *code);

#endif
