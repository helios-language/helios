
#include <AST.h>
#include <constant.h>
#include <instruction.h>
#include <program.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <token.h>

static void ast_postorderDFS(program *prgm, AST *ast) {
    for (uint32_t i = 0; i < ast->filled; i++) {
        ast_postorderDFS(prgm, ast->children[i]);
    }
    token_print(ast->value);
    printf("\n");

    switch (ast->value->t) {
    case TOK_DECINT: {
        uint32_t length = strlen((char *)(ast->value->value));
        uint8_t *res = malloc(length * sizeof(char));
        for (uint32_t i = 0; i < length; i++) {
            res[i] = ((char *)ast->value->value)[i] - '0';
        }
        uint64_t location = program_add_constant(prgm, length, res);
        program_add_instruction(prgm, LOAD_INT, location);
        break;
    }
    case TOK_BINARYOP: {
        program_add_instruction(prgm, ADD, 0);
        break;
    }

    case TOK_ROOTBLOCK: {
        break;
    }

    default:
        printf("instruction mapping for token ");
        token_print(ast->value);
        printf(" not found\n");
        exit(-1);
    }
}

program *generate_bytecode_from_ast(AST *ast) {
    program *res = program_new();

    ast_postorderDFS(res, ast);

    printf("\n\n\n");

    for (uint32_t i = 0; i < res->instructionfilled; i++) {
        printf("%i,", res->instructions[i].opcode);
        printf("%li", res->instructions[i].argument);
        printf("\n");
    }

    for (uint32_t i = 0; i < res->constantfilled; i++) {
        uint32_t size = res->constants[i].size;
        printf("%i,[", size);
        for (uint32_t j = 0; j < size; j++) {
            printf("%i", (uint32_t)(res->constants[i].bytes[j]));
        }
        printf("]\n");
    }

    return res;
}