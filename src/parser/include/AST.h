
#ifndef AST_H
#define AST_H

#include <stdint.h>
#include <token.h>

/**
 * the children array of an AST is actually an arraylist. it's default size is
 * whatever this variable is, but will increase as more children are added to an
 * ast.
 */
#define AST_DEFAULT_CHILDREN 2

/**
 * This struct represents an AST node object, and recursively defines an
 * AST.
 */
typedef struct AST {
    Token_t *value;  //!< the token stored in this node.
    uint32_t size;   //!< the allocated size of the children array.
    uint32_t filled; //!< the actual amount of used space in this array. Always
                     //!< less than the size attribute.
    struct AST **children; //!< the children array.
} AST;

AST *AST_new(Token_t *value);
void AST_free(AST *ast);
void AST_free_simple(AST *ast);
void AST_freeChildren(AST *ast);
void AST_addChild(AST *ast, AST *child);
AST *AST_get(AST *ast, uint32_t index);
void AST_set(AST *ast, uint32_t index, AST *child);

void AST_print(AST *ast);

#endif
