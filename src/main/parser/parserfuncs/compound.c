#include <AST.h>
#include <parser.h>
#include <parser_error.h>
#include <token.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DECLARE_PARSERFUNC(statement);

PARSERFUNC(block) { // '\r'?'\n' indent statement+ dedent

    uint32_t eslength1 = errorstack_length(parser->es);
    AST *result = AST_new(token_new(TOK_BLOCK, NULL));

    parser_expectchar(parser, '{');
    do {
        parser_skipws(parser);

        while (parser_acceptchar(parser, '\n')) {
            parser_skipws(parser);
        }

        if (parser_acceptchar(parser, '}')) {
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

/**
 * compound_statement = if block
 */
PARSERFUNC(compound_statement) {
    parser_skipws(parser);
    AST *res;
    if (parser_acceptstring(parser, "if")) {
        parser_skipws(parser);
        parser_expectchar(parser, ':');
        res = AST_new(token_new(TOK_IF_STMT, NULL));
    } else {
        return NULL;
    }
    AST_addChild(res, PARSERCALL(block));
    return res;
}