

#ifndef TOKEN_H
#define TOKEN_H

#include <stdint.h>

typedef enum {

    // integers
    TOK_DECINT,
    TOK_HEXINT,
    TOK_BININT,
    TOK_OCTINT,

    // float
    TOK_FLOAT,
    TOK_FPART,
    TOK_EPART,
    TOK_IPART,

    // operators
    TOK_BINARYOP,
    TOK_UNARYOP,

} TOKENTYPE;

typedef struct Token {
    TOKENTYPE t;
    void* value;
} Token_t;

void token_print(Token_t* token);
Token_t* token_new(TOKENTYPE t, void* value);
void token_free(Token_t* token);

#endif
