#include <enumtostring.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <token.h>

const char* tokennames[] = {
    enumtostring(TOK_DECINT),  enumtostring(TOK_HEXINT),
    enumtostring(TOK_BININT),  enumtostring(TOK_OCTINT),
    enumtostring(TOK_UNARYOP), enumtostring(TOK_BINARYOP),
    enumtostring(TOK_EPART),   enumtostring(TOK_FPART),
    enumtostring(TOK_IPART),   enumtostring(TOK_FLOAT),

};

void token_print(Token_t* token) {
    switch (token->t) {
        default:
            printf("Token <%s> : %s", tokennames[token->t],
                   (char*)token->value);
            break;
    }
}

Token_t* token_new(TOKENTYPE t, void* value) {
    Token_t* token = malloc(sizeof(Token_t));
    *token = (Token_t){t, value};
    return token;
}

void token_free(Token_t* token) {
    if (token != NULL) {
        free(token->value);
    }
    free(token);
}
