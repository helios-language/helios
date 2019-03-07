

#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>
#include <stdint.h>

/**
 *  enum representing all possible token types.
 */
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

    // lines
    TOK_NONE, // use when there's no real token, or it will eventually be
              // merged.
    TOK_BLOCK,

} TOKENTYPE;

/**
 * This struct represents a token object.
 */
typedef struct Token {
    TOKENTYPE t; //!< the type of this token
    void
        *value; //!< the value of this token. Void pointer to allow for any
                //!< datatype, but generally assumed to be a string. If it's not
                //!< a string the printing routine may error. For this a special
                //!< printing case can be defined in token.c (token_print).

    bool canfree; //!< true if the value is freeable
} Token_t;

void token_print(Token_t *token);
Token_t *token_new(TOKENTYPE t, void *value, bool canfree);
void token_free(Token_t *token);
void token_free_simple(Token_t *token);

#endif
