
#include <builtins.h>
#include <stdio.h>

int testmain(int argc, char *argv[]) {
    // helios_object *string_a = helios_string_from_charp("Hello World");
    // printf("%s\n",
    //        helios_string_to_charp(HELIOS_CALL_MEMBER(tostring, string_a)));

    // HELIOS_CALL_MEMBER(destructor, string_a);

    garbagecollector *gc = helios_init_garbagecollector();
    helios_set_garbagecollector(gc);

    for (uint32_t i = 0; i < 10000; i++) {
        helios_object *integer_a = helios_integer_from_cint(10);
        HELIOS_CALL_MEMBER(destructor, integer_a);
    }

    helios_delete_garbagecollector(gc);

    return 0;
}
