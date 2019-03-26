

#ifndef TOKEN_H
#define TOKEN_H

#include <builtins.h>
#include <stdbool.h>
#include <stdint.h>

/**
 *  enum representing all possible token types.
 */
typedef enum {

    // numbers
    TOK_INT,
    TOK_FLOAT,
    TOK_NONE,
    TOK_TRUE,
    TOK_FALSE,
    TOK_NAME,
    TOK_STRING,

    // unary operators
    TOK_UNARY_MINUS,
    TOK_UNARY_PLUS,
    TOK_UNARY_NOT,

    // binary operators
    TOK_PLUS,
    TOK_MINUS,
    TOK_DIVIDE,
    TOK_INTEGER_DIVIDE,
    TOK_MULTIPLY,
    TOK_POWER,
    TOK_ASSIGN,

    TOK_BLOCK,
    TOK_ROOTBLOCK,

    // statements
    TOK_IF_STMT,
    TOK_ELIF_STMT,
    TOK_ELSE_STMT,
    TOK_WHILE_STMT,
    TOK_SWITCH_STMT,
    TOK_FOR_STMT,

} TOKENTYPE;

/**
 * This struct represents a token object.
 */
typedef struct Token {
    TOKENTYPE t; //!< the type of this token
    helios_object *value;

} Token_t;

void token_print(Token_t *token);
Token_t *token_new(TOKENTYPE t, helios_object *value);
void token_free(Token_t *token);
void token_free_simple(Token_t *token);

#endif
