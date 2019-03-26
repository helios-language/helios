
#include <AST.h>
#include <error.h>
#include <parser.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Removes comments from a string. Note: frees old string and allocates new
 * string.
 *
 * In helios every character on a line, after a hashtag (#) is a comment.
 *
 * @param code the code with comments.
 * @return the transformed code without comments.
 */
char *parser_preprocess(char *code) {
    uint32_t size = 100;
    uint32_t filled = 0;
    char *res = malloc(size * sizeof(char));

    uint32_t indent = 0;
    uint32_t line = 1;

    for (char *i = code; *i != '\0'; i++) {
        if (*i == '#') {
            while ((*++i) != '\n')
                ;
        }
        if (*i == '\r') { // filter out carriage returns
            continue;
        }

        if (*i == PARSER_INDENT) { // DC1 will be indent so if it is in the code
                                   // before
            // applying indent syntax it has to be removed.
            continue;
        }
        if (*i == PARSER_DEDENT) { // DC2 will be dedent so if it is in the code
                                   // before
            // applying indent syntax it has to be removed.
            continue;
        }

        if (*i == '\n') {
            uint32_t n = 0;
            uint32_t spaces = 0;

            line++;

            while (1) {
                n++;
                char next = *(i + n);
                if (next == '\t') {
                    spaces += PARSER_SPACES_PER_TAB;
                    continue;
                } else if (next == ' ') {
                    spaces += 1;
                    continue;
                }
                break;
            }
            if (spaces % PARSER_SPACES_PER_TAB != 0) {
                printf("expected %i spaces per tab but found inconsistent "
                       "indent on line %i\n",
                       PARSER_SPACES_PER_TAB, line);
                exit(-1);
            }
            uint32_t new_indent = (uint32_t)(spaces / PARSER_SPACES_PER_TAB);

            // put the newline in the old place
            res[filled++] = '\n';
            if (filled >= size) {
                size <<= 1;
                res = realloc(res, size * sizeof(char));
            }

            if (new_indent < indent) {
                for (uint32_t counter = new_indent; counter < indent;
                     counter++) {
                    res[filled++] = PARSER_DEDENT;
                    if (filled >= size) {
                        size <<= 1;
                        res = realloc(res, size * sizeof(char));
                    }
                }
            } else if (new_indent > indent) {
                for (uint32_t counter = indent; counter < new_indent;
                     counter++) {
                    res[filled++] = PARSER_INDENT;
                    if (filled >= size) {
                        size <<= 1;
                        res = realloc(res, size * sizeof(char));
                    }
                }
            }

            i += (n - 1);
            indent = new_indent;

            continue;
        }

        res[filled++] = *i;
        if (filled >= size) {
            size <<= 1;
            res = realloc(res, size * sizeof(char));
        }
    }

    free(code);
    return res;
}

/**
 * Parser Entrypoint.
 * This is the base function for the parser. When it is called it will
 * construct the parser, and call the root of the recursive descent
 * algorithm.
 *
 * @param str The string to parse. Can obviously come from a file to parse
 * files. TODO: make function to read files and parse immediately
 * @return an abstract syntax tree 'object' (struct defined in AST.h)
 * which recursively represents the parsed syntax
 */
AST *parser_parseString(char *str) {
    str = parser_preprocess(str);

    printf("%s\n", str);

    Parser_t *parser = parser_new(str);

    parser_advance(parser);
    AST *ast = parser_root(parser);

    // this used to be in there to find cases where the statement ended
    // halfway through the compilation process to prevent it just exiting
    // without error. however, now the root of the recursive descent can
    // only exit errorless when it finds the end of the code. Because of
    // this i believe this code is not needed anymore. More testing is
    // required to verify this.

    // if (ast == NULL ||
    // !parser_exhausted(parser)) {
    //     errorstack_push(parser->es, "invalid syntax", parser->line,
    //                     parser->character);
    // }

    if (!errorstack_empty(parser->es)) {
        errorstack_traceback(parser->es, str);
    }

    parser_free(parser);

    return ast;
}

/**
 * Parser constructor.
 * This function allocates the space for a new parser object, and
 * initializes some of it's values.
 *
 * @param code Every parser is linked to some piece of code which it is
 * parsing. a pointer to this code (char *) is passed in every time a parser
 * is constructed
 * @return a newly made parser object.
 */
Parser_t *parser_new(char *code) {
    Parser_t *parser = malloc(sizeof(Parser_t));

    if (parser == NULL) {
        printf("malloc error\n");
        exit(-1);
    }

    uint32_t codelength = strlen(code);

    *parser = (Parser_t){
        errorstack_new(),
        1,    // line
        0,    // character
        '\0', // prev
        '\0', // curr
        '\0', // next
        code,
        codelength,                                        // codelength
        0,                                                 // index
        malloc(PARSER_ACCEPTED_MAX_STRLEN * sizeof(char)), // accepted
    };
    return parser;
}

/**
 * Parser destructor.
 * Destroys a parser object together with it's errorstack and string of
 * accepted characters.
 *
 * WARNING: this function will also delete the parser's errorstack. This
 * may not be desired when freeing copies of a parser made with the
 * parser_copy function as it will not copy the errorstack, only the
 * pointer to the errorstack. Deleting this stack might/will crash the
 * parser on subsequent accesses of the errorstack. Think about using the
 * parser_free_simple function.
 *
 * @param parser the parser to free
 */
void parser_free(Parser_t *parser) {
    errorstack_free(parser->es);
    free(parser->accepted);
    free(parser);
}

/**
 * Simple parser destructor.
 * Only frees the parser object, not the references to objects (like the
 * errorstack) that may be shared with other objects and thus will crash
 * the program. (see warning in the parser_free function's documentation)
 *
 * @param parser the parser to free
 */
void parser_free_simple(Parser_t *parser) {
    free(parser);
}

/**
 * Iterate the parser.
 * Updates the parser's prev, curr and next fields to represent the next
 * character in the parsed code. also updated the char and line field in
 * the parser based on the current scanned character (if it is a newline
 * the line field is incremented)
 *
 * @param parser the parser to iterate
 */
void parser_next(Parser_t *parser) {
    parser->prev = parser->curr;
    parser->curr = parser->next;
    if (parser->index < parser->codelength) {
        parser->next = parser->code[parser->index];
    } else {
        parser->next = '\0';
    }
    parser->character++;
    if (parser->curr == '\n') {
        parser->line++;
        parser->character = 0;
    }
    parser->index++;
}

/**
 * Test if the parser reached the end of the code.
 * Returns true when the current parser index is greater the length of
 * it's code.
 * @param parser the parser to test
 * @return if the parser has finished
 */
bool parser_exhausted(Parser_t *parser) {
    return parser->index > parser->codelength;
}

/**
 * Copy a parser.
 * Alternative constructor for a parser. Takes another parser and copies
 * all fields except the accepted string and the errorstack. Can be used in
 * conjunction with the parser_restore function to reset another parser to
 * the copied position. Remember to use parser_simple_free to not leave
 * copies of parsers behind in memory.
 *
 * @param parser the parser to copy
 * @return a copy of the parser
 */
Parser_t *parser_copy(Parser_t *parser) {
    Parser_t *newparser = malloc(sizeof(Parser_t));

    *newparser = (Parser_t){
        parser->es,
        parser->line,      // line
        parser->character, // character
        parser->prev,      // prev
        parser->curr,      // curr
        parser->next,      // next
        parser->code,
        parser->codelength, // codelength
        parser->index,      // index
        parser->accepted,   // accepted
    };
    return newparser;
}
/**
 * Restores a parser to another parser's position.
 * Copies over all parameters of another parser into this one except for
 * the errorstack and the accepted string. Only a pointer to those is
 * copied.

 * @param parser the parser to copy to
 * @param other the parser to take values from
 */
void parser_restore(Parser_t *parser, Parser_t *other) {
    *parser = (Parser_t){
        other->es,
        other->line,      // line
        other->character, // character
        other->prev,      // prev
        other->curr,      // curr
        other->next,      // next
        other->code,
        other->codelength, // codelength
        other->index,      // index
        other->accepted,   // accepted
    };
}

/**
 * Advances the parser as long as it can find whitespace.
 * @param parser the parser to advance
 */
void parser_skipws(Parser_t *parser) {
    while (parser->next != '\0' &&
           (parser->next == '\t' || parser->next == ' ')) {
        parser_advance(parser);
    }
}

/**
 * The same as parser_skipws except also skips newlines.
 * @param parser the parser to advances
 */
void parser_skipwsnl(Parser_t *parser) {
    while (
        parser->next != '\0' &&
        (parser->next == '\t' || parser->next == ' ' || parser->next == '\n')) {
        parser_advance(parser);
    }
}

/**
 * Proxy to parser_next. Mostly for legacy reasons.
 * TODO: remove this function and update all references to parser_next to
 * get a little bit more performance (although -O2 should already take care
 * of most of it)
 *
 * @param parser the parser to advance
 */
void parser_advance(Parser_t *parser) {
    parser_next(parser);
}

/**
 *    Searches for the given character, advances the parser if it found it.
 *  (looks only at parser->next)
 *
 *   @param parser the parser to search in
 *   @param character the character to search for
 *   @return boolean based on if it found the character or not
 *
 *  Note: updates parser->accepted with the character that was found
 */
bool parser_acceptchar(Parser_t *parser, char character) {
    if (parser->next != '\0' && parser->next == character) {
        parser_advance(parser);
        parser->accepted[0] = character;
        parser->accepted[1] = '\0';
        return true;
    }
    return false;
}

/**
 * Searches for any of the characters given in the parser. When it finds
 * any of the characters in the string in parser->next it advances the
 * parser.
 *
 * @param parser the parser to search in
 * @param characters the string of characters to search for
 * @return boolean based on if it found any of the characters in the
 * string
 *
 * Note: updates parser->accepted with the character that was found
 */
bool parser_acceptanychar(Parser_t *parser, char *characters) {
    for (uint32_t i = 0; characters[i] != '\0'; i++) {
        if (parser_acceptchar(parser, characters[i])) {
            parser->accepted[0] = characters[i];
            parser->accepted[1] = '\0';
            return true;
        }
    }
    return false;
}

/**
 * Advances the parser only if the next n characters (where n is the length
 * of the string) matches the string passed in. If it did not find this
 * string it reverts the parser to the same state as before the search.
 *
 * @param parser the parser to search in
 * @param str the string to search for
 * @return boolean based on if it found the string
 *
 * Note: updates parser->accepted with the string that was found.
 * WARNING: can only accept strings with a max length defined in parser.h
 * (PARSER_ACCEPTED_MAX_STRLEN). Exits the parser when this limit is
 * reached.
 * TODO: move constants like PARSER_ACCEPTED_MAX_STRLEN constants to a
 * config.h file that's imported in every c file.
 */
bool parser_acceptstring(Parser_t *parser, char *str) {
    Parser_t *parsercp1 = parser_copy(parser);
    for (uint32_t i = 0; str[i] != '\0'; i++) {
        if (!parser_acceptchar(parser, str[i])) {
            parser_restore(parser, parsercp1);
            parser_free_simple(parsercp1);
            return false;
        }
    }
    if (strlen(str) > PARSER_ACCEPTED_MAX_STRLEN - 1) {
        printf("ERROR accepted string was too long in parser_acceptstring, "
               "increase PARSER_ACCEPTED_MAX_STRLEN\n");
        exit(-1);
    }

    strcpy(parser->accepted, str);
    parser_free_simple(parsercp1);
    return true;
}

/**
 * If the parser can accept the given character, accept it, else push an
 * error on the parsers errorstack. and return false.
 *
 * @param parser the parser to check
 * @param character the character to check for
 * @return boolean based on if it found the character. If false is
 * returned the errorstack is enlarged by 1.
 */
bool parser_expectchar(Parser_t *parser, char character) {
    if (!parser_acceptchar(parser, character)) {
        errorstack_push(parser->es, "unexpected character", parser->line,
                        parser->character);
        return false;
    }
    return true;
}

/**
 * Crossover between acceptstring and expectchar. Pushes an error to the
 * errorstack if it cant find the given string in the next n characters
 * where n is the length of the string. if it could find it, accept id and
 * return true.
 *
 * @param parser the parser to check in
 * @param str the string to check for
 * @return boolean based on if it could find the given string
 */
bool parser_expectstring(Parser_t *parser, char *str) {
    Parser_t *parsercp1 = parser_copy(parser);
    for (uint32_t i = 0; str[i] != '\0'; i++) {
        if (!parser_expectchar(parser, str[i])) {
            errorstack_push(parser->es, "unexpected string", parser->line,
                            parser->character);
            parser_restore(parser, parsercp1);
            parser_free_simple(parsercp1); // free to avoid memory leaks
            return false;
        }
    }
    parser_free_simple(parsercp1); // free to avoid memory leaks
    return true;
}
