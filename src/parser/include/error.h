
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
typedef struct Error {
    uint32_t line;       //!< the line number of the occured error.
    uint32_t character;  //!< the character index of the occured error.
    const char *message; //!< the message associated with this error.
} Error_t;

/**
 * This struct represents a stack of errors. Whenever a new error is caused
 * its pushed to such a stack and then it may be ignored by the parser or
 * it's thrown. Whenever this happens the stack is walked down until its
 * empty and all errors on it are thrown, until the last one exits the
 * process. (this is the errorstack_traceback function in error.c)
 */
typedef struct Errorstack {
    uint32_t filled; //!< the amount of errors in the stack
    uint32_t size;   //!< the actual allocated size of the stack. Always larger
                     //!< than the filled parameter.
    Error_t *stack;
} Errorstack_t;

void error_print(Error_t err);
void error_throw(Error_t err, const char *code, bool hard);

Errorstack_t *errorstack_new();
void errorstack_free(Errorstack_t *es);
void errorstack_push(Errorstack_t *es, const char *message, uint32_t line,
                     uint32_t character);
uint32_t errorstack_length(Errorstack_t *es);
Error_t errorstack_pop(Errorstack_t *es);
void errorstack_popuntil(Errorstack_t *es, uint32_t length);
Error_t errorstack_top(Errorstack_t *es);
bool errorstack_empty(Errorstack_t *es);
void errorstack_traceback(Errorstack_t *es, const char *code);

#endif
