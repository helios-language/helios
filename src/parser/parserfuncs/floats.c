

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
 * Accepts a floating point constant in the format of
 *
 * TODO: accept exp aswell as e.
 */
PARSERFUNC(floatconst) {
    parser_skipws(parser);

    if (parser_acceptstring(parser, "Infinity")) {
        return AST_new(
            token_new(TOK_FLOAT, helios_float_from_cdouble(INFINITY)));
    } else if (parser_acceptstring(parser, "NaN")) {
        return AST_new(token_new(TOK_FLOAT, helios_float_from_cdouble(NAN)));
    } else {
        char *current = ((char *)parser->code - 1) + parser->index;

        char *end = NULL;
        double d = strtod(current, &end);
        if (d == 0.0 && (errno != 0 || end == current)) {
            errorstack_push(parser->es, "invalid float constant", parser->line,
                            parser->character);
            return AST_new(NULL);
        }

        bool founddecimalorexp = false;
        for (char *i = current; i < end; i++) {
            // a double must contain a . and an e
            if (*i == 'e' || *i == '.') {
                founddecimalorexp = true;
            }
            parser_advance(parser);
        }
        if (founddecimalorexp) {
            return AST_new(token_new(TOK_FLOAT, helios_float_from_cdouble(d)));
        } else {
            errorstack_push(parser->es, "invalid float constant", parser->line,
                            parser->character);
            return AST_new(NULL);
        }
    }
}
