

#include <builtins_test.h>

int builtins_test() {
    return integer_test() + float_test() + float_math_test() +
           integer_math_test();
}