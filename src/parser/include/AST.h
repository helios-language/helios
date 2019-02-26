
#ifndef AST_H
#define AST_H

#include <stdint.h>
#include <token.h>

#define AST_DEFAULT_CHILDREN 2

typedef struct AST {
    Token_t* value;
    uint32_t size;
    uint32_t filled;
    struct AST** children;
} AST_t;

AST_t* AST_new(Token_t* value);
void AST_free(AST_t* ast);
void AST_freeChildren(AST_t* ast);
void AST_addChild(AST_t* ast, AST_t* child);
AST_t* AST_get(AST_t* ast, uint32_t index);
void AST_set(AST_t* ast, uint32_t index, AST_t* child);

void AST_print(AST_t* ast);

#endif
