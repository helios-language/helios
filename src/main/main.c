
#include <stdio.h>

#include <AST.h>
#include <error.h>
#include <parser.h>

int main(int argc, char* argv[]) {
    AST_t* ast = parser_parseString("");
    AST_print(ast);
    AST_free(ast);
}
