#include <AST.h>
#include <helios_code.h>
#include <helios_memory.h>
#include <helios_object.h>
#include <helios_string.h>
#include <stdio.h>

void helios_code_from_AST_helper(helios_object *codeobject, AST *ast) {
    for (uint32_t i = 0; i < ast->filled; i++) {
        helios_code_from_AST_helper(codeobject, ast->children[i]);
    }
    switch (ast->value->t) {
    case TOK_ROOTBLOCK: {
        break;
    }
    case TOK_INT: // fall through
    case TOK_STRING:
    case TOK_FLOAT: {
        uint32_t location = helios_code_add_constant(
            codeobject, HELIOS_CALL_MEMBER(copy, ast->value->value));

        helios_code_add_instruction(codeobject,
                                    (helios_instruction){INSTR_LOD, location});
        break;
    }
    case TOK_MINUS: {
        helios_code_add_instruction(codeobject,
                                    (helios_instruction){INSTR_SUB, 0});
        break;
    }
    case TOK_MULTIPLY: {
        helios_code_add_instruction(codeobject,
                                    (helios_instruction){INSTR_MUL, 0});
        break;
    }
    case TOK_PLUS: {
        helios_code_add_instruction(codeobject,
                                    (helios_instruction){INSTR_ADD, 0});
        break;
    }
    case TOK_DIVIDE: {
        helios_code_add_instruction(codeobject,
                                    (helios_instruction){INSTR_DIV, 0});
        break;
    }
    default: {
        printf("could not generate bytecode - unknown token.\n");
        exit(-1);
    }
    }
}

helios_object *helios_code_from_AST(AST *ast) {
    helios_object *codeobject = helios_code_init();
    helios_code_from_AST_helper(codeobject, ast);
    helios_code_add_instruction(codeobject,
                                (helios_instruction){INSTR_STOP, 0});
    return codeobject;
}

void helios_code_add_instruction(helios_object *self,
                                 helios_instruction instr) {
    helios_code *obj = TO_HELIOS_CODE(self);
    obj->instructions[obj->instructionfilled++] = instr;
    if (obj->instructionfilled >= obj->instructionsize) {
        obj->instructionsize <<= 1;
        obj->instructions =
            realloc(obj->instructions,
                    obj->instructionsize * sizeof(helios_instruction));
    }
}

uint32_t helios_code_add_constant(helios_object *self,
                                  helios_object *constant) {
    helios_code *obj = TO_HELIOS_CODE(self);
    obj->constants[obj->constantfilled++] = constant;
    if (obj->constantfilled >= obj->constantsize) {
        obj->constantsize <<= 1;
        obj->constants = realloc(obj->constants,
                                 obj->constantsize * sizeof(helios_object *));
    }
    return obj->constantfilled - 1;
}

void helios_code_delete(helios_object *self) {
    free(TO_HELIOS_CODE(self)->constants);
    free(TO_HELIOS_CODE(self)->instructions);
    helios_free_object(self);
}

helios_object *helios_code_init() {
    helios_code *self =
        (helios_code *)helios_allocate_object(sizeof(helios_code));
    *self = (helios_code){
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_code_type),
        .constantsize = HELIOS_CODE_DEFAULT_CONSTANT_SIZE,
        .instructionsize = HELIOS_CODE_DEFAULT_INSTRUCTION_SIZE,
        .constantfilled = 0,
        .instructionfilled = 0,

        .constants =
            malloc(HELIOS_CODE_DEFAULT_CONSTANT_SIZE * sizeof(helios_object *)),
        .instructions = malloc(HELIOS_CODE_DEFAULT_INSTRUCTION_SIZE *
                               sizeof(helios_instruction)),
    };
    helios_set_garbagecollectable(TO_HELIOS_OBJECT(self));

    return TO_HELIOS_OBJECT(self);
}

helios_object *helios_code_tostring(helios_object *self) {
    return helios_string_from_charp("helios code object");
}

helios_object *helios_code_represent(helios_object *self) {
    return helios_code_tostring(self);
}

helios_object *helios_code_copy(helios_object *obj) {
    helios_code *old = TO_HELIOS_CODE(obj);
    helios_code *new = TO_HELIOS_CODE(helios_code_init());

    for (uint32_t i = 0; i < old->constantfilled; i++) {
        helios_code_add_constant(TO_HELIOS_OBJECT(new), old->constants[i]);
    }

    for (uint32_t i = 0; i < old->instructionfilled; i++) {
        helios_code_add_instruction(TO_HELIOS_OBJECT(new),
                                    old->instructions[i]);
    }
    return TO_HELIOS_OBJECT(new);
}

helios_type helios_code_type = {
    HELIOS_OBJECT_COMMON_BASE_INIT(&helios_object_type),

    HELIOS_OBJECT_BASIC_FIELDS_CONSTRUCTOR(helios_code_init),
    HELIOS_OBJECT_BASIC_FIELDS_DESTRUCTOR(helios_code_delete),
    HELIOS_OBJECT_BASIC_FIELDS_CLASSNAME("Code"),
    HELIOS_OBJECT_BASIC_FIELDS_REPRESENT(helios_code_tostring),
    HELIOS_OBJECT_BASIC_FIELDS_TOSTRING(helios_code_tostring),
    HELIOS_OBJECT_BASIC_FIELDS_COPY(helios_code_copy),

};