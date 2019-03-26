
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
 *The code in this file is mostly dedicated to accepting mathematical
 *expressions and applying the order of operations.
 */

DECLARE_PARSERFUNC(expr);
DECLARE_PARSERFUNC(factor);
DECLARE_PARSERFUNC(intconst);
DECLARE_PARSERFUNC(floatconst);
DECLARE_PARSERFUNC(stringconst);

PARSERFUNC(name) {
    parser_skipws(parser);

    char *res = malloc(10 * sizeof(char));
    uint32_t filled = 0;
    uint32_t size = 10;

    if (parser_acceptanychar(
            parser, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_")) {
        res[filled++] = parser->accepted[0];
        if (filled >= size) {
            size <<= 1;
            res = realloc(res, size * sizeof(char));
        }
        while (parser_acceptanychar(parser,
                                    "0123456789abcdefghijklmnopqrstuvwxyz"
                                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ_")) {
            res[filled++] = parser->accepted[0];
            if (filled >= size) {
                size <<= 1;
                res = realloc(res, size * sizeof(char));
            }
        }
        res[filled] = 0;

        AST *ast = AST_new(token_new(TOK_NAME, helios_string_from_charp(res)));
        free(res);
        return ast;
    } else {
        errorstack_push(parser->es, "invalid name", parser->line,
                        parser->character);
        return AST_new(NULL);
    }
}

/**
 * Tries to accept a floating point constant. if it couldn't find one it
 * tries an Integer constant, if it couldn't find one it tries a String
 * constant, if it couldnt find one etc... and returns this.
 *
 * TODO: All other constants (string, list etc)
 * NOTE: The order of float first int last is on purpose as the float
 * function will error and return much earlier. The int function first
 * checks for all different bases.
 *
 * rule:
 * atom = floatconst | intconst
 */
PARSERFUNC(atom) {
    parser_skipws(parser);
    uint32_t eslength1 = errorstack_length(parser->es);
    Parser_t *parsercp1 = parser_copy(parser);

    // string

    AST *res = PARSERCALL(stringconst);
    if (errorstack_length(parser->es) == eslength1) {
        parser_free_simple(parsercp1);
        return res;
    }
    AST_free(res);
    res = NULL;

    errorstack_popuntil(parser->es, eslength1);
    parser_restore(parser, parsercp1);

    // float

    res = PARSERCALL(floatconst);
    if (errorstack_length(parser->es) == eslength1) {
        parser_free_simple(parsercp1);
        return res;
    }
    AST_free(res);
    res = NULL;

    errorstack_popuntil(parser->es, eslength1);
    parser_restore(parser, parsercp1);

    // name

    res = PARSERCALL(name);
    if (errorstack_length(parser->es) == eslength1) {
        parser_free_simple(parsercp1);
        return res;
    }
    AST_free(res);
    res = NULL;

    errorstack_popuntil(parser->es, eslength1);
    parser_restore(parser, parsercp1);

    // int

    parser_free_simple(parsercp1);
    return PARSERCALL(intconst);
}

/**
 * Either accepts an atom or a bracketed expression
 *
 * rule:
 * bracketexpression = ("(" expr ")") | atom
 */
PARSERFUNC(bracketexpression) {
    parser_skipws(parser);

    if (parser_acceptchar(parser, '(')) {
        parser_skipws(parser);
        AST *res = PARSERCALL(expr);
        parser_skipws(parser);
        parser_expectchar(parser, ')');
        parser_skipws(parser);
        return res;
    }
    return PARSERCALL(atom);
}
/**
 *  Accepts parts of expressions which can have exponent notation (a ** b)
 * in them.
 *
 * rule:
 * power = bracketexpression ["**" power]
 */
PARSERFUNC(power) {
    parser_skipws(parser);
    AST *left = PARSERCALL(bracketexpression);
    AST *res = left;
    parser_skipws(parser);
    if (parser_acceptstring(parser, "**")) {
        parser_skipws(parser);
        char *op = malloc(strlen(parser->accepted) + 1);
        strcpy(op, parser->accepted);
        AST *right = PARSERCALL(factor);
        parser_skipws(parser);

        AST *res = NULL;
        if (strcmp(op, "**") == 0) {
            res = AST_new(token_new(TOK_POWER, helios_string_from_charp(op)));
        } else {
            free(op);
            errorstack_push(parser->es, "invalid operator bbb", parser->line,
                            parser->character);
            return AST_new(NULL);
        }
        free(op);
        AST_addChild(res, left);
        AST_addChild(res, right);
        return res;
    }
    parser_skipws(parser);
    return res;
}

/**
 * Accepts parts of expressions containing unary operators like + - and ~
 * to negate, invert or make positive a part of an expression.
 *
 * rule:
 * factor = (("+" | "-" | "~") factor) | power
 */
PARSERFUNC(factor) {
    parser_skipws(parser);
    if (parser_acceptanychar(parser, "+-~")) {
        parser_skipws(parser);
        char *op = malloc(strlen(parser->accepted) + 1);
        strcpy(op, parser->accepted);
        AST *child = PARSERCALL(factor);
        parser_skipws(parser);
        AST *res = NULL;
        if (strcmp(op, "+") == 0) {
            res = AST_new(token_new(TOK_UNARY_PLUS, NULL));
        } else if (strcmp(op, "-") == 0) {
            res = AST_new(token_new(TOK_UNARY_MINUS, NULL));
        } else if (strcmp(op, "~") == 0) {
            res = AST_new(token_new(TOK_UNARY_NOT, NULL));
        } else {
            free(op);
            errorstack_push(parser->es, "invalid operator aaa", parser->line,
                            parser->character);
            return AST_new(NULL);
        }
        free(op);
        AST_addChild(res, child);
        return res;
    }
    return PARSERCALL(power);
}

/**
 * Accepts parts of expressions containing operators like * and / or
 * operators with the same precedance.
 *
 * rule:
 * term = factor (("*" | "/" | "//") factor)*
 *
 * TODO: add modulo
 */
PARSERFUNC(term) {
    parser_skipws(parser);
    AST *left = PARSERCALL(factor);
    AST *res = left;
    parser_skipws(parser);
    while (parser_acceptstring(parser, "//") ||
           parser_acceptchar(parser, '*') || parser_acceptchar(parser, '/')) {
        char *op = malloc(strlen(parser->accepted) + 1);
        strcpy(op, parser->accepted);
        AST *right = PARSERCALL(factor);
        if (strcmp(op, "//") == 0) {
            res = AST_new(
                token_new(TOK_INTEGER_DIVIDE, helios_string_from_charp(op)));
        } else if (strcmp(op, "*") == 0) {
            res =
                AST_new(token_new(TOK_MULTIPLY, helios_string_from_charp(op)));
        } else if (strcmp(op, "/") == 0) {
            res = AST_new(token_new(TOK_DIVIDE, helios_string_from_charp(op)));
        } else {
            errorstack_push(parser->es, "invalid operator", parser->line,
                            parser->character);
            if (res == NULL) {
                res = AST_new(NULL);
            }
            break;
        }
        free(op);
        AST_addChild(res, left);
        AST_addChild(res, right);
        left = res;
    }
    return res;
}

/**
 * Accepts any full expression.
 *
 * rule:
 * expr = term (("+" | "-") term)*
 */
PARSERFUNC(expr) {
    parser_skipws(parser);
    AST *left = PARSERCALL(term);
    AST *res = left;
    parser_skipws(parser);
    while (parser_acceptchar(parser, '+') || parser_acceptchar(parser, '-')) {
        char *op = malloc(strlen(parser->accepted) + 1);
        strcpy(op, parser->accepted);
        AST *right = PARSERCALL(term);
        if (strcmp(op, "+") == 0) {
            res = AST_new(token_new(TOK_PLUS, helios_string_from_charp(op)));
        } else if (strcmp(op, "-") == 0) {
            res = AST_new(token_new(TOK_MINUS, helios_string_from_charp(op)));
        } else {
            errorstack_push(parser->es, "invalid operator ccc", parser->line,
                            parser->character);
            if (res == NULL) {
                res = AST_new(NULL);
            }
            free(op);
            break;
        }
        free(op);
        AST_addChild(res, left);
        AST_addChild(res, right);
        left = res;
    }
    return res;
}
