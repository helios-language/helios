
#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>
#include <stdint.h>

#define ERRORSTACK_DEFAULT_SIZE 2

typedef struct Error {
    uint32_t line;
    uint32_t character;
    const char* message;
} Error_t;

typedef struct Errorstack {
    uint32_t filled;
    uint32_t size;
    Error_t* stack;
} Errorstack_t;

void error_print(Error_t err);
void error_throw(Error_t err, const char* code, bool hard);

Errorstack_t* errorstack_new();
void errorstack_free(Errorstack_t* es);
void errorstack_push(Errorstack_t* es,
                     const char* message,
                     uint32_t line,
                     uint32_t character);
uint32_t errorstack_length(Errorstack_t* es);
Error_t errorstack_pop(Errorstack_t* es);
void errorstack_popuntil(Errorstack_t* es, uint32_t length);
Error_t errorstack_top(Errorstack_t* es);
bool errorstack_empty(Errorstack_t* es);
void errorstack_traceback(Errorstack_t* es, const char* code);

#endif
