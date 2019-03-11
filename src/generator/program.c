
#include <constant.h>
#include <instruction.h>
#include <program.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

program *program_new() {
    program *prgm = malloc(sizeof(program));
    *prgm = (program){
        PROGRAM_DEFAULT_INSTRUCTION_SIZE,
        0,
        PROGRAM_DEFAULT_CONSTANT_SIZE,
        0,
        HELIOS_PROGRAM_MAGIC,
        malloc(PROGRAM_DEFAULT_INSTRUCTION_SIZE * sizeof(instruction)),
        malloc(PROGRAM_DEFAULT_CONSTANT_SIZE * sizeof(constant)),
    };

    return prgm;
}

void program_delete(program *prgm) {
    for (uint32_t i = 0; i < prgm->constantfilled; i++) {
        free(prgm->constants[i].bytes);
    }
    free(prgm->constants);
    free(prgm->instructions);
    free(prgm);
}

uint64_t program_add_constant(program *prgm, uint32_t size, uint8_t *bytes) {
    prgm->constants[prgm->constantfilled++] = (constant){size, bytes};

    if (prgm->constantfilled >= prgm->constantsize) {
        prgm->constantsize <<= 1;
        prgm->constants =
            realloc(prgm->constants, prgm->constantsize * sizeof(instruction));
    }
    return prgm->constantfilled - 1;
}

void program_add_instruction(program *prgm, OPCODE opc, uint64_t argument) {
    prgm->instructions[prgm->instructionfilled++] =
        (instruction){argument, opc};
    if (prgm->instructionfilled >= prgm->instructionsize) {
        prgm->instructionsize <<= 1;
        prgm->instructions = realloc(
            prgm->instructions, prgm->instructionsize * sizeof(instruction));
    }
}