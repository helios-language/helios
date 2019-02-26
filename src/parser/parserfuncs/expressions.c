
#include <AST.h>
#include <error.h>
#include <parser.h>
#include <token.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DECLARE_PARSERFUNC(expr);
DECLARE_PARSERFUNC(intconst);
DECLARE_PARSERFUNC(floatconst);

// self.skipws()
// length = ErrorStack.getCurrentLength()
// codecp1 = self.code.copy()

// res = self.floatconst()
// if ErrorStack.getCurrentLength() == length:
//     return res

// ErrorStack.popUntilLength(length)
// self.code = codecp1.copy()

// res = self.intconst()
// if ErrorStack.getCurrentLength() == length:
//     return res

// # ErrorStack.popUntilLength(length)
// self.code = codecp1.copy()

// ErrorStack.error(Error(
//     "could not parse atom",
//     self.code.line, self.code.char
// ))
// return AST("")

PARSERFUNC(atom) {
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
            AST_t* res = AST_new(token_new(TOK_UNARYOP, op));
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
    parser_skipws(parser);
    if (parser_acceptanychar(parser, "+-~")) {
        parser_skipws(parser);
        char* op = malloc(strlen(parser->accepted) + 1);
        strcpy(op, parser->accepted);
        AST_t* child = PARSERCALL(factor);
        parser_skipws(parser);
        if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 ||
            strcmp(op, "~") == 0) {
            AST_t* res = AST_new(token_new(TOK_UNARYOP, op));
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
            res = AST_new(token_new(TOK_BINARYOP, op));
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
    parser_skipws(parser);
    AST_t* left = PARSERCALL(term);
    AST_t* res = left;
    parser_skipws(parser);
    while (parser_acceptchar(parser, '+') || parser_acceptchar(parser, '-')) {
        char* op = malloc(strlen(parser->accepted) + 1);
        strcpy(op, parser->accepted);
        AST_t* right = PARSERCALL(term);
        if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) {
            res = AST_new(token_new(TOK_BINARYOP, op));
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
