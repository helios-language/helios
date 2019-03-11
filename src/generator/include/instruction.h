

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>

/**
 * This enum represents all the different possible opcodes there are
 */
typedef enum {
    LOAD_INT, //!< loads an int on the top of the stack with the value at
    ADD,      //<! pops TOS and TOS-1, adds them and puts them on the stack.
    SUBTRACT, //<! pops TOS and TOS-1, subtracts them and puts them on the
              // stack.
} OPCODE;

typedef struct {
    uint64_t argument; //<! the argument to this function.
    OPCODE opcode;     //<! unque id representing this instruction
} instruction;

#endif
