
#include <stdio.h>
#include <stdlib.h>

#include <AST.h>
#include <error.h>
#include <parser.h>

#include <files.h>

#include <test.h>

int main(int argc, char* argv[]) {
    printf("%i\n",argc);
    if(argc < 2){
        printf("please give the helios compiler a file to compile.\n");
        exit(-1);
    }
    char * file = readfiletostring(argv[1]);

    AST_t* ast = parser_parseString(file);
    AST_print(ast);
    AST_free(ast);

    free(file);
}
