
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

// PARSERFUNC(block) {
//     // indent line+ dedent

// }

PARSERFUNC(simple_statement) {
    return PARSERCALL(expr);
}

PARSERFUNC(statement) {
    // statement:  simple_statement (";" simple_statement)* ";"? _nl
    AST *ast = AST_new(token_new(TOK_NONE, (char *)"", false));
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

    parser_acceptchar(parser, '\r');
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
 * ("\n" | "\r" | statement)* end.
 */
PARSERFUNC(root) {
    uint32_t eslength1 = errorstack_length(parser->es);
    AST *result = AST_new(token_new(TOK_ROOTBLOCK, "rootblock", false));

    do {
        parser_skipws(parser);

        while (parser_acceptanychar(parser, "\r\n")) {
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
