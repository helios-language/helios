
#ifndef PARSER_H
#define PARSER_H

#include <AST.h>
#include <error.h>
#include <stdbool.h>
#include <stdint.h>

/*
    use these macros only inside functions belonging to the recursive descent
    parsing:
*/

// defines a function used for recursive descent parsing. accepts a parser
// object and returns an ast object.
#define PARSERFUNC(name) AST_t* parser_##name(Parser_t* parser)
// similar to parserfunc, except only forward-declares it.
#define DECLARE_PARSERFUNC(name) AST_t* parser_##name(Parser_t* parser)
// calls a parserfunc with the desired arguments (the parser).
#define PARSERCALL(name) parser_##name(parser)

/*
    The maximum length of the parsers accepted character string.
*/
#define PARSER_ACCEPTED_MAX_STRLEN 100

typedef struct Parser {
    /*
        This struct represents the state of the parser.
    */
    Errorstack_t* es;    // the errorstack belonging to this parser
    uint32_t line;       // the line the parser is currently on
    uint32_t character;  // the character in this line the parser is currently
                         // pointing at.

    char prev;  // the last accepted character.
    char curr;  // the character the index of this parser currently points at.
    char next;  // the next character which can be accepted.

    char* code;           // the string of code currently parsed.
    uint32_t codelength;  // the length of this code - precomputed.
    uint32_t index;  // the index in this code which is currently scanned by the
                     // parser.

    char* accepted;  // a string of whatever the previous call to
                     // accept[char,anychar,string] has found so it can be added
                     // to an ast.

    uint32_t indent;  // the current indentation level
} Parser_t;

Parser_t* parser_new(char* code);

// destroys errorstack
void parser_free(Parser_t* parser);

// doesnt destroy the errorstack referenced by this parser
void parser_free_simple(Parser_t* parser);

void parser_next(Parser_t* parser);
bool parser_exhausted(Parser_t* parser);
Parser_t* parser_copy(Parser_t* parser);
void parser_restore(Parser_t* parser, Parser_t* other);
AST_t* parser_parseString(char* str);
AST_t* parser_root(Parser_t* parser);

void parser_skipws(Parser_t* parser);
void parser_skipwsnl(Parser_t* parser);
void parser_advance(Parser_t* parser);
bool parser_acceptchar(Parser_t* parser, char character);
bool parser_acceptanychar(Parser_t* parser, char* characters);
bool parser_acceptstring(Parser_t* parser, char* str);
bool parser_expectchar(Parser_t* parser, char character);
bool parser_expectstring(Parser_t* parser, char* str);

#endif
