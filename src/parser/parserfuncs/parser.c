
#include <AST.h>
#include <error.h>
#include <parser.h>
#include <token.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DECLARE_PARSERFUNC(name);
DECLARE_PARSERFUNC(expr);
DECLARE_PARSERFUNC(compound_statement);

PARSERFUNC(single_statement) {
    AST *res = NULL;

    parser_skipws(parser);

    uint32_t eslength = errorstack_length(parser->es);
    AST *left = PARSERCALL(expr);
    if (errorstack_length(parser->es) > eslength) {
        errorstack_popuntil(parser->es, eslength);
        errorstack_push(parser->es, "expected name", parser->line,
                        parser->character);
        return AST_new(NULL);
    }

    parser_skipws(parser);
    if (parser_acceptchar(parser, '=')) {
        res = AST_new(token_new(TOK_ASSIGN, NULL));
    }
    parser_skipws(parser);
    AST *right = PARSERCALL(expr);

    if (res != NULL) {
        AST_addChild(res, left);
        AST_addChild(res, right);
        return res;
    } else {
        return NULL;
    }
}

PARSERFUNC(simple_statement) {
    AST *res = PARSERCALL(compound_statement);
    if (res != NULL) {
        return res;
    }
    AST_free(res);
    return PARSERCALL(single_statement);
}

/**
 * statement:  simple_statement (";" simple_statement)* ";"? \n
 *
 */
PARSERFUNC(statement) {
    AST *ast = AST_new(token_new(TOK_NONE, NULL));
    parser_skipws(parser);
    AST_addChild(ast, PARSERCALL(simple_statement));
    parser_skipws(parser);
    while (parser_acceptchar(parser, ';')) {
        parser_skipws(parser);

        uint32_t length1 = errorstack_length(parser->es);
        AST *nextstmt = PARSERCALL(simple_statement);
        if (length1 != errorstack_length(parser->es)) {
            AST_free(nextstmt);
            errorstack_popuntil(parser->es, length1);
            break;
        }
        AST_addChild(ast, nextstmt);
    }
    parser_skipws(parser);
    while (parser_acceptchar(parser, ';'))
        ;

    parser_expectchar(parser, '\n');
    return ast;
}

/**
 * root parsing function.
 *
 * TODO: i feel like this is kind of a hacky way of doing it (checking for
 * parser_exhausted here). maybe there's a better way?? but it seems to
 * work fine i guess???
 *
 * rule:
 * ("\n" | statement)* end.
 */
PARSERFUNC(root) {
    uint32_t eslength1 = errorstack_length(parser->es);
    AST *result = AST_new(token_new(TOK_ROOTBLOCK, NULL));

    do {
        parser_skipws(parser);

        while (parser_acceptchar(parser, '\n')) {
            parser_skipws(parser);
        }

        if (parser_exhausted(parser)) {
            break;
        }

        AST *tmp = PARSERCALL(statement);
        for (int i = 0; i < tmp->filled; i++) {
            AST_addChild(result, tmp->children[i]);
        }
        AST_free_simple(tmp);
    } while (eslength1 == errorstack_length(parser->es));
    return result;
}
