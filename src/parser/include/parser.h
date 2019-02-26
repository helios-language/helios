
#ifndef PARSER_H
#define PARSER_H

#include <AST.h>
#include <error.h>
#include <stdbool.h>
#include <stdint.h>

#define PARSERFUNC(name) AST_t* parser_##name(Parser_t* parser)
#define DECLARE_PARSERFUNC(name) AST_t* parser_##name(Parser_t* parser)
#define PARSERCALL(name) parser_##name(parser)

#define PARSER_ACCEPTED_MAX_STRLEN 100

typedef struct Parser {
    Errorstack_t* es;
    uint32_t line;
    uint32_t character;

    char prev;
    char curr;
    char next;

    char* code;
    uint32_t codelength;
    uint32_t index;

    char* accepted;
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
