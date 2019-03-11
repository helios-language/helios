/**
 *
 *  The magic number obviously is 4242
 *
 *
 *  program layout:
 *  header:
 *  (uint16_t)MAGICNUMBER (uint64_t)CODELENGTH
 *
 *  code in (uint16_t)opcode (uint64_t)argument pairs. The argument may be
 *  omitted for certain instructions. constants
 *  constants
 */

#ifndef PROGRAM_H
#define PROGRAM_H

#define HELIOS_PROGRAM_MAGIC ((uint64_t)0x4242)

#include <constant.h>
#include <instruction.h>
#include <stdint.h>

#define PROGRAM_DEFAULT_INSTRUCTION_SIZE 10
#define PROGRAM_DEFAULT_CONSTANT_SIZE 10

typedef struct {
    uint64_t instructionsize;
    uint64_t instructionfilled;
    uint64_t constantsize;
    uint64_t constantfilled;

    uint16_t magic;

    instruction *instructions;
    constant *constants;
} program;

program *program_new();
void program_delete(program *prgm);
uint64_t program_add_constant(program *prgm, uint32_t size, uint8_t bytes[]);
void program_add_instruction(program *prgm, OPCODE opc, uint64_t argument);

#endif