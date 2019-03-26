
#include <builtins.h>
#include <executestack.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void hvm_execute_codeobject(helios_object *code_temp) {
    execstack *stack = execstack_init();
    helios_code *code = TO_HELIOS_CODE(code_temp);
    uint32_t instructionpointer = 0;
    bool notstopped = true;

    while (notstopped) {
        // debug
        printf("executestack before: ");
        execstack_print(stack);

        helios_instruction_print(&code->instructions[instructionpointer]);
        // debug end

        uint32_t argument = code->instructions[instructionpointer].argument;

        switch (code->instructions[instructionpointer].opcode) {
        case INSTR_LOD: {
            execstack_push(stack, code->constants[argument]);
            break;
        }
        case INSTR_ADD: {
            helios_object *right = execstack_pop(stack);
            helios_object *left = execstack_pop(stack);
            if (HELIOS_HAS_MEMBER(add, left)) {
                execstack_push(stack, HELIOS_CALL_MEMBER(add, left, right));
                HELIOS_DECREF(left);
                HELIOS_DECREF(right);
            } else {
                printf("addition not supported for object %s\n",
                       right->class->classname);
                exit(-1);
            }
            break;
        }
        case INSTR_DIV: {
            helios_object *right = execstack_pop(stack);
            helios_object *left = execstack_pop(stack);
            if (HELIOS_HAS_MEMBER(add, left)) {
                execstack_push(stack, HELIOS_CALL_MEMBER(divide, left, right));
                HELIOS_DECREF(left);
                HELIOS_DECREF(right);
            } else {
                printf("divison not supported for object %s\n",
                       right->class->classname);
                exit(-1);
            }
            break;
        }
        case INSTR_SUB: {
            helios_object *right = execstack_pop(stack);
            helios_object *left = execstack_pop(stack);
            if (HELIOS_HAS_MEMBER(add, left)) {
                execstack_push(stack, HELIOS_CALL_MEMBER(divide, left, right));
                HELIOS_DECREF(left);
                HELIOS_DECREF(right);
            } else {
                printf("divison not supported for object %s\n",
                       right->class->classname);
                exit(-1);
            }
            break;
        }
        case INSTR_MUL: {
            helios_object *right = execstack_pop(stack);
            helios_object *left = execstack_pop(stack);
            if (HELIOS_HAS_MEMBER(add, left)) {
                execstack_push(stack, HELIOS_CALL_MEMBER(divide, left, right));
                HELIOS_DECREF(left);
                HELIOS_DECREF(right);
            } else {
                printf("divison not supported for object %s\n",
                       right->class->classname);
                exit(-1);
            }
            break;
        }
        case INSTR_STOP: {
            notstopped = false;
            break;
        }
        default: {
            printf("instruction not recognised\n");
            exit(-1);
            break;
        }
        }

        instructionpointer++;
        if (instructionpointer > code->instructionfilled) {
            printf("Instruction pointer out of bounds but no stop instruction"
                   "found. Is your code object corrupted?\n");
            exit(-1);
        }
    }

    execstack_delete(stack);
}
