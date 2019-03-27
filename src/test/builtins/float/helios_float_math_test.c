#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>

#include <builtins.h>

int helios_float_math_test_setup(void **state) {
    garbagecollector *gc = helios_init_garbagecollector();
    helios_set_garbagecollector(gc);
    return 0;
}

int helios_float_math_test_teardown(void **state) {
    helios_delete_garbagecollector(helios_get_garbagecollector());
    return 0;
}

int float_math_test() {
    const struct CMUnitTest tests[] = {

    };
    return cmocka_run_group_tests_name("Helios float math", tests,
                                       helios_float_math_test_setup,
                                       helios_float_math_test_teardown);
}
