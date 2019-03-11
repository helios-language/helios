
#ifndef GENERATOR_H
#define GENERATOR_H

#include <AST.h>
#include <program.h>

program *generate_bytecode_from_ast(AST *ast);

#endif