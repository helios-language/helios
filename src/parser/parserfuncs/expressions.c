
#include <AST.h>
#include <error.h>
#include <parser.h>
#include <token.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
The code in this file is mostly dedicated to accepting mathematical expressions
and applying the order of operations.
*/

DECLARE_PARSERFUNC(expr);
DECLARE_PARSERFUNC(intconst);
DECLARE_PARSERFUNC(floatconst);

PARSERFUNC(atom) {
    /*
        Tries to accept a floating point constant. if it couldn't find one it
        tries an Integer constand and returns this.

        TODO: All other constants (string, list etc)
        NOTE: The order of float first int last is on purpose as the float
        function will error and return much earlier. The int function first
        checks for all different bases.

        rule:
        atom = floatconst | intconst
    */
    parser_skipws(parser);
    uint32_t eslength1 = errorstack_length(parser->es);
    Parser_t* parsercp1 = parser_copy(parser);

    AST_t* res = PARSERCALL(floatconst);
    if (errorstack_length(parser->es) == eslength1) {
        parser_free_simple(parsercp1);
        return res;
    }
    errorstack_popuntil(parser->es, eslength1);
    parser_restore(parser, parsercp1);
    parser_free_simple(parsercp1);

    return PARSERCALL(intconst);
}

PARSERFUNC(atomexpression) {
    /*
        Either accepts an atom or a bracketed expression

        rule:
        atomexpression = ("(" expr ")") | atom
    */
    parser_skipws(parser);

    if (parser_acceptchar(parser, '(')) {
        parser_skipws(parser);
        AST_t* res = PARSERCALL(expr);
        parser_skipws(parser);
        parser_expectchar(parser, ')');
        parser_skipws(parser);
        return res;
    }
    return PARSERCALL(atom);
}

PARSERFUNC(power) {
    /*
        Accepts parts of expressions which can have exponent notation (a ** b)
        in them.

        rule:
        power = atomexpression ["**" power]
     */
    parser_skipws(parser);
    AST_t* left = PARSERCALL(atomexpression);
    AST_t* res = left;
    parser_skipws(parser);
    if (parser_acceptstring(parser, "**")) {
        parser_skipws(parser);
        char* op = malloc(strlen(parser->accepted) + 1);
        strcpy(op, parser->accepted);
        AST_t* right = PARSERCALL(power);
        parser_skipws(parser);

        if (strcmp(op, "**") == 0) {
            AST_t* res = AST_new(token_new(TOK_UNARYOP, op, true));
            AST_addChild(res, left);
            AST_addChild(res, right);
            return res;
        } else {
            errorstack_push(parser->es, "invalid operator bbb", parser->line,
                            parser->character);
            return AST_new(NULL);
        }
    }
    parser_skipws(parser);
    return res;
}

PARSERFUNC(factor) {
    /*
        Accepts parts of expressions containing unary operators like + - and ~
        to negate, invert or make positive a part of an expression.
        rule:
        factor = (("+" | "-" | "~") factor) | power
    */
    parser_skipws(parser);
    if (parser_acceptanychar(parser, "+-~")) {
        parser_skipws(parser);
        char* op = malloc(strlen(parser->accepted) + 1);
        strcpy(op, parser->accepted);
        AST_t* child = PARSERCALL(factor);
        parser_skipws(parser);
        if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 ||
            strcmp(op, "~") == 0) {
            AST_t* res = AST_new(token_new(TOK_UNARYOP, op, true));
            AST_addChild(res, child);
            return res;
        } else {
            errorstack_push(parser->es, "invalid operator aaa", parser->line,
                            parser->character);
            return AST_new(NULL);
        }
    }
    return PARSERCALL(power);
}

PARSERFUNC(term) {
    /*
        Accepts parts of expressions containing operators like * and / or
        operators with the same precedance.

        rule:
        term = factor (("*" | "/" | "//") factor)*

        TODO: add modulo
    */
    parser_skipws(parser);
    AST_t* left = PARSERCALL(factor);
    AST_t* res = left;
    parser_skipws(parser);
    while (parser_acceptstring(parser, "//") ||
           parser_acceptchar(parser, '*') || parser_acceptchar(parser, '/')) {
        char* op = malloc(strlen(parser->accepted) + 1);
        strcpy(op, parser->accepted);
        AST_t* right = PARSERCALL(factor);
        if (strcmp(op, "//") == 0 || strcmp(op, "*") == 0 ||
            strcmp(op, "/") == 0) {
            res = AST_new(token_new(TOK_BINARYOP, op, true));
            AST_addChild(res, left);
            AST_addChild(res, right);
            left = res;
        } else {
            errorstack_push(parser->es, "invalid operator eee", parser->line,
                            parser->character);
            if (res == NULL) {
                res = AST_new(NULL);
            }
        }
    }
    return res;
}

PARSERFUNC(expr) {
    /*
        Accepts any full expression.

        rule:
        expr = term (("+" | "-") term)*
    */
    parser_skipws(parser);
    AST_t* left = PARSERCALL(term);
    AST_t* res = left;
    parser_skipws(parser);
    while (parser_acceptchar(parser, '+') || parser_acceptchar(parser, '-')) {
        char* op = malloc(strlen(parser->accepted) + 1);
        strcpy(op, parser->accepted);
        AST_t* right = PARSERCALL(term);
        if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) {
            res = AST_new(token_new(TOK_BINARYOP, op, true));
            AST_addChild(res, left);
            AST_addChild(res, right);
            left = res;
        } else {
            errorstack_push(parser->es, "invalid operator ccc", parser->line,
                            parser->character);
            if (res == NULL) {
                res = AST_new(NULL);
            }
        }
    }
    return res;
}
