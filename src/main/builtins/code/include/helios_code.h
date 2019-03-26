
#ifndef HELIOS_CODE_H
#define HELIOS_CODE_H

#include <helios_object.h>

#define HELIOS_CODE_DEFAULT_CONSTANT_SIZE 10
#define HELIOS_CODE_DEFAULT_INSTRUCTION_SIZE 10

// forward declaration
struct __AST_s;

typedef enum {
    INSTR_LOD,
    INSTR_ADD,
    INSTR_SUB,
    INSTR_DIV,
    INSTR_MUL,

    INSTR_STOP, //!< end of code
} helios_instruction_opcode;

typedef struct {
    helios_instruction_opcode opcode;
    uint32_t argument;
} helios_instruction;

typedef struct __helios_code_s {
    HELIOS_OBJECT_COMMON_BASE;
    uint32_t instructionfilled;
    uint32_t instructionsize;

    helios_instruction *instructions;

    uint32_t constantfilled;
    uint32_t constantsize;

    helios_object **constants;

    // args
    // kwargs
    // returns

} helios_code;

helios_type helios_code_type;

//! cast any generic helios object to a helios_integer object.
#define TO_HELIOS_CODE(obj) ((struct __helios_code_s *)obj)
//! return 1/true (c int) if obj is an instance of helios_integer. Note:
//! subclasses aren't covered.
#define IS_HELIOS_CODE(obj) (obj->class == &helios_code_type)

void helios_code_delete(helios_object *self);
helios_object *helios_code_init();
uint32_t helios_code_add_constant(helios_object *self, helios_object *constant);
void helios_code_add_instruction(helios_object *self, helios_instruction instr);
helios_object *helios_code_from_AST(struct __AST_s *ast);

helios_object *helios_code_tostring(helios_object *self);
helios_object *helios_code_represent(helios_object *self);
helios_object *helios_code_copy(helios_object *obj);

// helios_instruction.c
void helios_instruction_print(helios_instruction *instruction);

#endif