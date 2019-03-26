

#include <AST.h>
#include <error.h>
#include <parser.h>
#include <token.h>

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Parses a string with a veriable ending char to work with both string
 * variations.
 */
static AST *string_internal(Parser_t *parser, char endchar) {
    char *res = malloc(10 * sizeof(char));
    uint32_t filled = 0;
    uint32_t size = 10;
    bool escape = false;
    while (!(parser->next == endchar && !escape)) {
        if (parser->next == '\\' && !escape) {
            escape = true;

        } else {
            escape = false;
            res[filled++] = parser->next;
            if (filled >= size) {
                size <<= 1;
                res = realloc(res, size * sizeof(char));
            }
        }

        parser_advance(parser);
    }
    parser_advance(parser);
    res[filled] = 0;

    AST *ast = AST_new(token_new(TOK_STRING, helios_string_from_charp(res)));
    free(res);
    return ast;
}

/**
 * parses a helios string.
 */
PARSERFUNC(stringconst) {
    if (parser_acceptchar(parser, '\"')) {
        return string_internal(parser, '\"');
    } else if (parser_acceptchar(parser, '\'')) {
        return string_internal(parser, '\'');
    } else {
        errorstack_push(parser->es, "invalid string constant", parser->line,
                        parser->character);
        return AST_new(NULL);
    }
}