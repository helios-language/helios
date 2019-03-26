#include <builtins.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <token.h>

/**
 * macro which expands in a way that it can define values at indices of an
 * array. enumtostring(10) will expand to [10] = "10", which sets index 10 of an
 * array to the string "10". Useful for enum names.
 */
#define enumtostring(x) [x] = #x

/**
 * this array consists of all the possible enum values of the TOKENTYPE enum,
 * and their string representation. Enumtostring automatically inserts the right
 * values.
 */
const char *tokennames[] = {
    enumtostring(TOK_INT),         enumtostring(TOK_FLOAT),
    enumtostring(TOK_TRUE),        enumtostring(TOK_FALSE),
    enumtostring(TOK_NAME),        enumtostring(TOK_NONE),
    enumtostring(TOK_STRING),      enumtostring(TOK_BLOCK),
    enumtostring(TOK_ROOTBLOCK),

    enumtostring(TOK_DIVIDE),      enumtostring(TOK_INTEGER_DIVIDE),
    enumtostring(TOK_PLUS),        enumtostring(TOK_MINUS),
    enumtostring(TOK_MULTIPLY),    enumtostring(TOK_POWER),

    enumtostring(TOK_UNARY_MINUS), enumtostring(TOK_UNARY_NOT),
    enumtostring(TOK_UNARY_PLUS),

    enumtostring(TOK_IF_STMT),     enumtostring(TOK_ELIF_STMT),
    enumtostring(TOK_ELSE_STMT),   enumtostring(TOK_WHILE_STMT),
    enumtostring(TOK_SWITCH_STMT), enumtostring(TOK_FOR_STMT),

};

/**
 * Prints a token object.
 * @param token the token to print.
 */
void token_print(Token_t *token) {
    switch (token->t) {
    default:
        if (token->value != NULL) {
            helios_object *string = HELIOS_CALL_MEMBER(represent, token->value);
            char *res = helios_string_to_charp(string);
            printf("Token <%s> : %s", tokennames[token->t], res);
            free(res);
            HELIOS_CALL_MEMBER(destructor, string);
        } else {
            printf("Token <%s> : NULL", tokennames[token->t]);
        }

        break;
    }
}

/**
 * Constructor for token objects.
 * @param t the type of the constructed token in enum TOKENTYPE.
 * @param value the value of this token.
 * @return the newly created token.
 *  WARNING: never use string constants as values for tokens
 */
Token_t *token_new(TOKENTYPE t, helios_object *value) {
    Token_t *token = malloc(sizeof(Token_t));
    *token = (Token_t){t, value};
    return token;
}

/**
 * Destroys a token object.Also frees the token's value.
 *
 * @param token the token to free
 */
void token_free(Token_t *token) {
    if (token == NULL) {
        return;
    }
    if (token->value != NULL) {
        HELIOS_CALL_MEMBER(destructor, token->value);
    }
    free(token);
}
