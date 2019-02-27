
#include <AST.h>
#include <error.h>
#include <parser.h>
#include <token.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PARSERFUNC(binconst) {
    /*
        Accepts a binary constant.

        rule:
        binconst = 0b("0" | "1")+
    */
    if (!parser_expectstring(parser, "0b")) {
        errorstack_push(parser->es, "invalid bin constant", parser->line,
                        parser->character);
        return AST_new(NULL);
    }

    if (!parser_acceptanychar(parser, "01")) {
        errorstack_push(parser->es, "invalid bin constant", parser->line,
                        parser->character);
        return AST_new(NULL);
    }
    uint32_t filled = 1;
    uint32_t size = 2;
    char* res = malloc(size);
    res[0] = parser->accepted[0];
    while (parser_acceptanychar(parser, "01")) {
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

    return AST_new(token_new(TOK_BININT, res));
}

PARSERFUNC(hexconst) {
    /*
        Accepts a hexadecimal constant.

        Also accepts uppercase letters for a-f.

        rule:
        hexconst = 0x("0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
       | "a" | "b" | "c" | "d" | "e" | "f" | "A" | "B" | "C" | "D" | "E" | "F")+
    */
    if (!parser_expectstring(parser, "0x")) {
        errorstack_push(parser->es, "invalid hex constant", parser->line,
                        parser->character);
        return AST_new(NULL);
    }

    if (!parser_acceptanychar(parser, "0123456789abcdefABCDEF")) {
        errorstack_push(parser->es, "invalid hex constant", parser->line,
                        parser->character);
        return AST_new(NULL);
    }
    uint32_t filled = 1;
    uint32_t size = 2;
    char* res = malloc(size);
    res[0] = parser->accepted[0];
    while (parser_acceptanychar(parser, "0123456789abcdefABCDEF")) {
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

    return AST_new(token_new(TOK_HEXINT, res));
}

PARSERFUNC(octconst) {
    /*
        Accepts an octal constant.

        rule:
        octconst = 0o("0" | "1" | "2" | "3" | "4" | "5" | "6" | "7")+
    */
    if (!parser_expectstring(parser, "0o")) {
        errorstack_push(parser->es, "invalid octal constant", parser->line,
                        parser->character);
        return AST_new(NULL);
    }

    if (!parser_acceptanychar(parser, "01234567")) {
        errorstack_push(parser->es, "invalid octal constant", parser->line,
                        parser->character);
        return AST_new(NULL);
    }
    uint32_t filled = 1;
    uint32_t size = 2;
    char* res = malloc(size);
    res[0] = parser->accepted[0];
    while (parser_acceptanychar(parser, "01234567")) {
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

    return AST_new(token_new(TOK_OCTINT, res));
}

PARSERFUNC(decconst) {
    /*
        Accepts a decimal constant.

        rule:
        decconst = [0d]("0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" |
       "9")+
    */
    parser_acceptstring(parser, "0d");
    if (!parser_acceptanychar(parser, "0123456789")) {
        errorstack_push(parser->es, "invalid decimal constant", parser->line,
                        parser->character);
        return AST_new(NULL);
    }
    uint32_t filled = 1;
    uint32_t size = 2;
    char* res = malloc(size);
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

    return AST_new(token_new(TOK_DECINT, res));
}

PARSERFUNC(intconst) {
    /*
        Accepts an integer of any (supported) base.
        rule:
        intconst = hexconst | octconst | binconst | decconst
    */
    // setup
    parser_skipws(parser);

    uint32_t eslength = errorstack_length(parser->es);
    Parser_t* parsercp1 = parser_copy(parser);

    AST_t* res;

    // // HEX

    res = PARSERCALL(hexconst);
    if (errorstack_length(parser->es) == eslength) {
        parser_free_simple(
            parsercp1);  // free the parser copy to avoid memory leaks
        return res;
    }
    AST_free(res);  // free old AST as it no longer represents the actual value
    // since the error stack grew
    res = NULL;
    errorstack_popuntil(parser->es, eslength);
    parser_restore(parser, parsercp1);

    // OCT

    res = PARSERCALL(octconst);
    if (errorstack_length(parser->es) == eslength) {
        parser_free_simple(
            parsercp1);  // free the parser copy to avoid memory leaks
        return res;
    }
    AST_free(res);  // free old AST as it no longer represents the actual value
    // since the error stack grew
    res = NULL;
    errorstack_popuntil(parser->es, eslength);
    parser_restore(parser, parsercp1);

    // // BIN

    res = PARSERCALL(binconst);
    if (errorstack_length(parser->es) == eslength) {
        parser_free_simple(
            parsercp1);  // free the parser copy to avoid memory leaks
        return res;
    }
    AST_free(res);  // free old AST as it no longer represents the actualvalue
    // since the error stack grew
    res = NULL;
    errorstack_popuntil(parser->es, eslength);
    parser_restore(parser, parsercp1);

    // DECIMAL
    res = PARSERCALL(decconst);
    if (errorstack_length(parser->es) == eslength) {
        parser_free_simple(
            parsercp1);  // free the parser copy to avoid memory leaks
        return res;
    }
    AST_free(res);  // free old AST as it no longer represents the actual value
                    // since the error stack grew
    res = NULL;
    errorstack_popuntil(parser->es, eslength);
    parser_restore(parser, parsercp1);

    // none of the above
    parser_free_simple(parsercp1);
    errorstack_push(parser->es, "invalid integer constant", parser->line,
                    parser->character);
    return AST_new(NULL);
}
