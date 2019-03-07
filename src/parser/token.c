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
    enumtostring(TOK_DECINT),  enumtostring(TOK_HEXINT),
    enumtostring(TOK_BININT),  enumtostring(TOK_OCTINT),
    enumtostring(TOK_UNARYOP), enumtostring(TOK_BINARYOP),
    enumtostring(TOK_EPART),   enumtostring(TOK_FPART),
    enumtostring(TOK_IPART),   enumtostring(TOK_FLOAT),
    enumtostring(TOK_NONE),    enumtostring(TOK_BLOCK),

};

/**
 * Prints a token object.
 * @param token the token to print.
 */
void token_print(Token_t *token) {
    switch (token->t) {
    default:
        printf("Token <%s> : %s", tokennames[token->t], (char *)token->value);
        break;
    }
}

/**
 * Constructor for token objects.
 * @param t the type of the constructed token in enum TOKENTYPE.
 * @param value the value of this token.
 * @param canfree true if the value is freeable
 * @return the newly created token.
 *  WARNING: never use string constants as values for tokens
 */
Token_t *token_new(TOKENTYPE t, void *value, bool canfree) {
    Token_t *token = malloc(sizeof(Token_t));
    *token = (Token_t){t, value, canfree};
    return token;
}

void token_free(Token_t *token) {
    /**
     * Destroys a token object.Also frees the token's value.
     *
     * @param token the token to free
     */

    if (token != NULL && token->canfree) {
        free(token->value);
    }
    free(token);
}
