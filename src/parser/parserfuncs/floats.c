

#include <AST.h>
#include <error.h>
#include <parser.h>
#include <token.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Similar to the function accepting integers but does not accept the 0d
 * prefix.
 *
 * rule:
 * number = ("0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9")+
 */
PARSERFUNC(number) {
    if (!parser_acceptanychar(parser, "0123456789")) {
        errorstack_push(parser->es, "invalid decimal constant", parser->line,
                        parser->character);
        return AST_new(NULL);
    }
    uint32_t filled = 1;
    uint32_t size = 2;
    char *res = malloc(size);
    res[0] = parser->accepted[0];
    while (parser_acceptanychar(parser, "0123456789")) {
        res[filled++] = parser->accepted[0];
        if (filled > size - 1) {
            size <<= 1;
            res = realloc(res, size);
        }
    }
    // not sure if this is necessary and will ever be executed
    if (filled > size - 1) {
        size++;
        res = realloc(res, size);
    }

    res[filled] = '\0';

    return AST_new(token_new(TOK_DECINT, res, true));
}

/**
 * Accepts a floating point constant in the format of
 *
 * rule:
 * floatconst = (number "." number [("exp" | "e") number]) |
 * number ("exp" | "e") number
 */
PARSERFUNC(floatconst) {
    parser_skipws(parser);

    uint32_t eslength1 = errorstack_length(parser->es);
    Parser_t *parsercp1 = parser_copy(parser);

    AST_t *result = AST_new(
        token_new(TOK_FLOAT, (void *)"(intpart,floatpart,exponent)", false));

    AST_t *ipart = PARSERCALL(number);
    if (errorstack_length(parser->es) != eslength1) {
        AST_free(ipart);
        ipart = AST_new(token_new(TOK_IPART, (void *)"0", false));
        errorstack_popuntil(parser->es, eslength1);
        parser_restore(parser, parsercp1);
    }

    AST_addChild(result, ipart);

    parser_skipws(parser);
    if (!parser_expectchar(parser, '.')) {
        // if it couldnt find a ".", still check for the exponent
        parser_skipws(parser);
        if (parser_acceptstring(parser, "exp") ||
            parser_acceptstring(parser, "e")) {
            errorstack_popuntil(parser->es, eslength1);

            parser_skipws(parser);

            AST_t *epart = PARSERCALL(number);
            if (errorstack_length(parser->es) == eslength1) {
                AST_addChild(result,
                             AST_new(token_new(TOK_FPART, (void *)"0", false)));
                AST_addChild(result, epart);
                parser_free_simple(parsercp1);
                return result;
            }
        }
        errorstack_push(parser->es, "invalid float constant", parser->line,
                        parser->character);
        parser_restore(parser, parsercp1);
        parser_free_simple(parsercp1);
        AST_free(result);
        return AST_new(NULL);
    } else {
        ipart->value->t = TOK_FPART;
    }
    uint32_t eslength2 = errorstack_length(parser->es);
    Parser_t *parsercp2 = parser_copy(parser);

    AST_t *fpart = PARSERCALL(number);
    if (errorstack_length(parser->es) != eslength2) {
        AST_free(fpart);
        fpart = AST_new(token_new(TOK_FPART, (void *)"0", false));
        errorstack_popuntil(parser->es, eslength2);
        parser_restore(parser, parsercp2);
    } else {
        fpart->value->t = TOK_FPART;
    }

    AST_addChild(result, fpart);
    parser_free_simple(parsercp2);

    parser_skipws(parser);
    AST_t *epart = NULL;

    if (parser_acceptstring(parser, "exp") ||
        parser_acceptstring(parser, "e")) {
        parser_skipws(parser);

        epart = PARSERCALL(number);
        if (errorstack_length(parser->es) != eslength1) {
            errorstack_push(parser->es, "invalid float constant after exponent",
                            parser->line, parser->character);
            AST_free(result);
            parser_restore(parser, parsercp1);
            parser_free_simple(parsercp1);
            return AST_new(NULL);
        }
        epart->value->t = TOK_EPART;
    } else {
        epart = AST_new(token_new(TOK_EPART, (void *)"0", false));
    }

    AST_addChild(result, epart);

    parser_free_simple(parsercp1);
    return result;
}
