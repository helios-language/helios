
#ifndef AST_H
#define AST_H

#include <stdint.h>
#include <token.h>

// the children array of an AST is actually an arraylist. it's default size is
// whatever this variable is, but will increase as more children are added to an
// ast.
#define AST_DEFAULT_CHILDREN 2

typedef struct AST {
    /*
        This struct represents an AST node object, and recursively defines an
        AST.
    */
    Token_t* value;   // the token stored in this node.
    uint32_t size;    // the allocated size of the children array.
    uint32_t filled;  // the actual amount of used space in this array. Always
                      // less than the size attribute.
    struct AST** children;  // the children array.
} AST_t;

AST_t* AST_new(Token_t* value);
void AST_free(AST_t* ast);
void AST_free_simple(AST_t* ast);
void AST_freeChildren(AST_t* ast);
void AST_addChild(AST_t* ast, AST_t* child);
AST_t* AST_get(AST_t* ast, uint32_t index);
void AST_set(AST_t* ast, uint32_t index, AST_t* child);

void AST_print(AST_t* ast);

#endif
