
#include <AST.h>
#include <helios_code.h>
#include <helios_memory.h>
#include <helios_object.h>
#include <helios_string.h>
#include <stdio.h>

#define enumtostring(x) [x] = #x

const char *instructionnames[] = {
    enumtostring(INSTR_ADD), enumtostring(INSTR_DIV), enumtostring(INSTR_MUL),
    enumtostring(INSTR_SUB), enumtostring(INSTR_LOD), enumtostring(INSTR_STOP),
};

void helios_instruction_print(helios_instruction *instruction) {
    printf("%s %d\n", instructionnames[instruction->opcode],
           instruction->argument);
}