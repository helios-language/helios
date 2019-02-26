
#include <stdio.h>

#include <AST.h>
#include <error.h>
#include <parser.h>

int main(int argc, char* argv[]) {
    AST_t* ast = parser_parseString("0.1 + 1 * 0b01 **0o3 / 0x2");
    AST_print(ast);
    AST_free(ast);
}
