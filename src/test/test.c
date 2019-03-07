
#include <builtins.h>
#include <stdio.h>

int testmain(int argc, char *argv[]) {
    helios_object *integer_a = helios_integer_from_cint(10);
    helios_object *integer_b = helios_integer_from_cint(11);

    helios_object *integer_c = CALL_HELIOS_INTEGER(add, integer_a, integer_b);
    helios_object *integer_d =
        CALL_HELIOS_INTEGER(subtract, integer_a, integer_b);
    helios_object *integer_e =
        CALL_HELIOS_INTEGER(multiply, integer_a, integer_b);
    helios_object *integer_f =
        CALL_HELIOS_INTEGER(divide, integer_a, integer_b);

    printf("%i\n", TO_HELIOS_INTEGER(integer_a)->value);
    printf("%i\n", TO_HELIOS_INTEGER(integer_b)->value);

    printf("+ %i\n", TO_HELIOS_INTEGER(integer_c)->value);
    printf("- %i\n", TO_HELIOS_INTEGER(integer_d)->value);
    printf("* %i\n", TO_HELIOS_INTEGER(integer_e)->value);
    printf("/ %i\n", TO_HELIOS_INTEGER(integer_f)->value);

    CALL_HELIOS_INTEGER(destructor, integer_a);
    CALL_HELIOS_INTEGER(destructor, integer_b);
    CALL_HELIOS_INTEGER(destructor, integer_c);
    CALL_HELIOS_INTEGER(destructor, integer_d);
    CALL_HELIOS_INTEGER(destructor, integer_e);
    CALL_HELIOS_INTEGER(destructor, integer_f);
}
