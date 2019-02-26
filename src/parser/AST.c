
#include <AST.h>
#include <parser.h>
#include <token.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

AST_t* AST_new(Token_t* value) {
    AST_t* ast = malloc(sizeof(AST_t));
    *ast = (AST_t){value, AST_DEFAULT_CHILDREN, 0,
                   malloc(AST_DEFAULT_CHILDREN * sizeof(AST_t*))};
    return ast;
}

void AST_free(AST_t* ast) {
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

void AST_freeChildren(AST_t* ast) {
    if (ast == NULL) {
        return;
    }
    for (uint32_t i = 0; i < ast->filled; i++) {
        if (ast->children[i] != NULL) {
            AST_free(ast->children[i]);
        }
    }
}

void AST_addChild(AST_t* ast, AST_t* child) {
    ast->children[ast->filled++] = child;
    if (ast->filled >= ast->size) {
        ast->size <<= 1;
        ast->children = realloc(ast->children, ast->size * sizeof(AST_t*));
    }
}

AST_t* AST_get(AST_t* ast, uint32_t index) {
    if (index >= ast->filled || index < 0) {
        printf("ast error: child index out of bounds in AST_get");
        exit(-1);
    }
    return ast->children[index];
}

void AST_set(AST_t* ast, uint32_t index, AST_t* child) {
    if (index >= ast->filled || index < 0) {
        printf("ast error: child index out of bounds in AST_set");
        exit(-1);
    }
    ast->children[index] = child;
}

void AST_print_helper(AST_t* ast, uint32_t depth) {
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

void AST_print(AST_t* ast) {
    AST_print_helper(ast, 0);
}
