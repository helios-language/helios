
#include <stdio.h>
#include <stdlib.h>

#include <AST.h>
#include <error.h>
#include <generator.h>
#include <parser.h>

#include <files.h>

int main(int argc, char *argv[]) {
    printf("%i\n", argc);
    if (argc < 3) {
        printf("please give the helios compiler a file to compile and a file "
               "to write to.\n");
        exit(-1);
    }
    char *file = readfiletostring(argv[1]);

    AST *ast = parser_parseString(file);
    AST_print(ast);

    printf("\n\n\n");
    program *prgm = generate_bytecode_from_ast(ast);

    writeprogramtofile(argv[2], prgm);

    AST_free(ast);
    program_delete(prgm);

    free(file);

    return 0;
}
