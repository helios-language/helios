
#include <AST.h>
#include <error.h>
#include <parser.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AST_t* parser_parseString(char* str) {
    Parser_t* parser = parser_new(str);

    parser_advance(parser);
    AST_t* ast = parser_root(parser);

    if (ast == NULL || !parser_exhausted(parser)) {
        errorstack_push(parser->es, "invalid syntax", parser->line,
                        parser->character);
    }

    if (!errorstack_empty(parser->es)) {
        errorstack_traceback(parser->es, str);
    }

    parser_free(parser);

    return ast;
}

Parser_t* parser_new(char* code) {
    Parser_t* parser = malloc(sizeof(Parser_t));
    *parser = (Parser_t){
        errorstack_new(),
        0,     // line
        -1,    // character
        '\0',  // prev
        '\0',  // curr
        '\0',  // next
        code,
        strlen(code),                                      // codelength
        0,                                                 // index
        malloc(PARSER_ACCEPTED_MAX_STRLEN * sizeof(char))  // accepted
    };
    return parser;
}

// WARNING only use this when you are ACTUALLY freeing the parser. it will also
// destroy the errorstack
void parser_free(Parser_t* parser) {
    errorstack_free(parser->es);
    free(parser->accepted);
    free(parser);
}

// simple free doesnt remove the errorstack as it is still referenced by a copy
// sometimes
void parser_free_simple(Parser_t* parser) {
    free(parser);
}

void parser_next(Parser_t* parser) {
    parser->prev = parser->curr;
    parser->curr = parser->next;
    if (parser->index < parser->codelength) {
        parser->next = parser->code[parser->index];
    } else {
        parser->next = '\0';
    }
    parser->character++;
    if (parser->next == '\n') {
        parser->line++;
        parser->character = 0;
    }
    parser->index++;
}

bool parser_exhausted(Parser_t* parser) {
    return parser->index > parser->codelength;
}

Parser_t* parser_copy(Parser_t* parser) {
    Parser_t* newparser = malloc(sizeof(Parser_t));
    if (newparser == NULL) {
        printf("malloc returned NULL in %s %s:%i\n", __FUNCTION__, __FILE__,
               __LINE__);
        exit(-1);
    }
    *newparser = (Parser_t){
        parser->es,
        parser->line,       // line
        parser->character,  // character
        parser->prev,       // prev
        parser->curr,       // curr
        parser->next,       // next
        parser->code,
        parser->codelength,  // codelength
        parser->index,       // index
        parser->accepted     // accepted
    };
    return newparser;
}

void parser_restore(Parser_t* parser, Parser_t* other) {
    *parser = (Parser_t){
        other->es,
        other->line,       // line
        other->character,  // character
        other->prev,       // prev
        other->curr,       // curr
        other->next,       // next
        other->code,
        other->codelength,  // codelength
        other->index,       // index
        other->accepted     // accepted
    };
}

void parser_skipws(Parser_t* parser) {
    while (parser->next != '\0' &&
           (parser->next == '\t' || parser->next == ' ')) {
        parser_advance(parser);
    }
}

void parser_skipwsnl(Parser_t* parser) {
    while (
        parser->next != '\0' &&
        (parser->next == '\t' || parser->next == ' ' || parser->next == '\n')) {
        parser_advance(parser);
    }
}

void parser_advance(Parser_t* parser) {
    parser_next(parser);
}

bool parser_acceptchar(Parser_t* parser, char character) {
    if (parser->next != '\0' && parser->next == character) {
        parser_advance(parser);
        parser->accepted[0] = character;
        parser->accepted[1] = '\0';
        return true;
    }
    return false;
}

bool parser_acceptanychar(Parser_t* parser, char* characters) {
    for (uint32_t i = 0; characters[i] != '\0'; i++) {
        if (parser_acceptchar(parser, characters[i])) {
            parser->accepted[0] = characters[i];
            parser->accepted[1] = '\0';
            return true;
        }
    }
    return false;
}

bool parser_acceptstring(Parser_t* parser, char* str) {
    Parser_t* parsercp1 = parser_copy(parser);
    for (uint32_t i = 0; str[i] != '\0'; i++) {
        if (!parser_acceptchar(parser, str[i])) {
            parser_restore(parser, parsercp1);
            parser_free_simple(parsercp1);
            return false;
        }
    }
    if (strlen(str) > PARSER_ACCEPTED_MAX_STRLEN - 1) {
        printf(
            "ERROR accepted string was too long in parser_acceptstring, "
            "increase PARSER_ACCEPTED_MAX_STRLEN\n");
        exit(-1);
    }

    strcpy(parser->accepted, str);
    parser_free_simple(parsercp1);
    return true;
}

bool parser_expectchar(Parser_t* parser, char character) {
    if (!parser_acceptchar(parser, character)) {
        errorstack_push(parser->es, "unexpected character", parser->line,
                        parser->character);
        return false;
    }
    return true;
}

bool parser_expectstring(Parser_t* parser, char* str) {
    Parser_t* parsercp1 = parser_copy(parser);
    for (uint32_t i = 0; str[i] != '\0'; i++) {
        if (!parser_expectchar(parser, str[i])) {
            errorstack_push(parser->es, "unexpected string", parser->line,
                            parser->character);
            parser_restore(parser, parsercp1);
            parser_free_simple(parsercp1);  // free to avoid memory leaks
            return false;
        }
    }
    parser_free_simple(parsercp1);  // free to avoid memory leaks
    return true;
}
