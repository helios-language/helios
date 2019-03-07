
#include <builtins.h>
#include <stdio.h>

int testmain(int argc, char *argv[]) {
    helios_object *string_a = helios_string_from_charp("Hello World");
    printf("%s\n", helios_string_to_charp(HELIOS_CALL_MEMBER(represent, string_a)));

    // helios_object *integer_a = helios_integer_from_cint(10);
    // helios_object *integer_b = helios_integer_from_cint(11);

    // helios_object *integer_c = HELIOS_CALL_MEMBER(add, integer_a,
    // integer_b); helios_object *integer_d =
    //     HELIOS_CALL_MEMBER(subtract, integer_a, integer_b);
    // helios_object *integer_e =
    //     HELIOS_CALL_MEMBER(multiply, integer_a, integer_b);
    // helios_object *integer_f =
    //     HELIOS_CALL_MEMBER(divide, integer_a, integer_b);

    // printf("%i\n", TO_HELIOS_INTEGER(integer_a)->value);
    // printf("%i\n", TO_HELIOS_INTEGER(integer_b)->value);

    // printf("+ %i\n", TO_HELIOS_INTEGER(integer_c)->value);
    // printf("- %i\n", TO_HELIOS_INTEGER(integer_d)->value);
    // printf("* %i\n", TO_HELIOS_INTEGER(integer_e)->value);
    // printf("/ %i\n", TO_HELIOS_INTEGER(integer_f)->value);

    // HELIOS_CALL_MEMBER(destructor, integer_a);
    // HELIOS_CALL_MEMBER(destructor, integer_b);
    // HELIOS_CALL_MEMBER(destructor, integer_c);
    // HELIOS_CALL_MEMBER(destructor, integer_d);
    // HELIOS_CALL_MEMBER(destructor, integer_e);
    // HELIOS_CALL_MEMBER(destructor, integer_f);
}


