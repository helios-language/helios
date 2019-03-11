
#include <AST.h>
#include <parser.h>
#include <token.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Constructore for ast objects.
 *
 * @param value the token to put in this ast
 * @return the created ast.
 */
AST *AST_new(Token_t *value) {
    AST *ast = malloc(sizeof(AST));
    *ast = (AST){value, AST_DEFAULT_CHILDREN, 0,
                 malloc(AST_DEFAULT_CHILDREN * sizeof(AST *))};
    return ast;
}

/**
 * Destructor for ast objects. Recursively frees an AST to its leaf nodes.
 * @param ast the ast to destroy
 */
void AST_free(AST *ast) {
    if (ast == NULL) {
        return;
    }
    for (uint32_t i = 0; i < ast->filled; i++) {
        if (ast->children[i] != NULL) {
            AST_free(ast->children[i]);
        }
    }
    token_free(ast->value);
    ast->value = NULL;
    free(ast->children);
    ast->children = NULL;
    free(ast);
}

/**
 * Destructor for ast objects. DOESNT recursively destroy children and only
 * touches the ast node it received.
 * @param ast the ast to destroy
 */
void AST_free_simple(AST *ast) {
    if (ast == NULL) {
        return;
    }

    token_free(ast->value);
    ast->value = NULL;
    free(ast->children);
    ast->children = NULL;
    free(ast);
}

/**
 * Destructor for an AST. ONLY children are freed but the root node is
 * kept.
 * @param ast the ast which children will be freed.
 */
void AST_freeChildren(AST *ast) {
    if (ast == NULL) {
        return;
    }
    for (uint32_t i = 0; i < ast->filled; i++) {
        if (ast->children[i] != NULL) {
            AST_free(ast->children[i]);
        }
    }
}

/**
 * Adds a child ast node to a parent ast node.
 * @param ast the parent the child is added to
 * @param child the child node which gets attached to the parent
 */
void AST_addChild(AST *ast, AST *child) {
    ast->children[ast->filled++] = child;
    if (ast->filled >= ast->size) {
        ast->size <<= 1;
        ast->children = realloc(ast->children, ast->size * sizeof(AST *));
    }
}

/**
 * Get the nth child of an AST.
 * @param ast the ast to get the nth child of.
 * @param index the index of the child that is returned.
 */
AST *AST_get(AST *ast, uint32_t index) {
    if (index >= ast->filled || index < 0) {
        printf("ast error: child index out of bounds in AST_get");
        exit(-1);
    }
    return ast->children[index];
}
/**
 * Set (overwrite) the nth node of the AST. Note: this nth node isnt
 * automatically freed when overwritten and can cause memory leaks.
 * @param ast the ast to set a node of
 * @param index the index to replace a node at
 * @param child the ast which becomes the new child at the given index
 */
void AST_set(AST *ast, uint32_t index, AST *child) {
    if (index >= ast->filled || index < 0) {
        printf("ast error: child index out of bounds in AST_set");
        exit(-1);
    }
    ast->children[index] = child;
}

/**
 * Recursively prints an ast. For internal use only as it adds a depth
 * parameter.
 * @param ast the ast to print
 * @param depth the recursive depth of the printing and as such,
 * proportional to the indentation of the printed nodes.
 */
void AST_print_helper(AST *ast, uint32_t depth) {
    for (uint32_t i = 0; i < depth; i++) {
        printf("  ");
    }
    if (ast == NULL) {
        printf("NULL\n");
        return;
    } else {
        token_print(ast->value);
    }

    printf("\n");
    for (uint32_t i = 0; i < ast->filled; i++) {
        AST_print_helper(ast->children[i], depth + 1);
    }
}

/**
 * Print an ast object. This is the function that's visible from outside
 * this c module. It will call the helper function with 0 as second
 * argument to start the indentation at 0.
 * @param ast the ast to print
 */
void AST_print(AST *ast) {
    AST_print_helper(ast, 0);
}
