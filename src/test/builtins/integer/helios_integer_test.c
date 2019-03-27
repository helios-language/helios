#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>

#include <builtins.h>

void helios_integer_init_test(void **state) {
    helios_object *obj = helios_integer_init();

    assert_string_equal("Integer", obj->class->classname);

    helios_integer *intobj = TO_HELIOS_INTEGER(obj);
    assert_int_equal(intobj->value, HELIOS_INTEGER_DEFAULT_VALUE);
}

void helios_integer_from_cint_test(void **state) {
    helios_integer *obj = TO_HELIOS_INTEGER(helios_integer_from_cint(10));
    assert_int_equal(obj->value, 10);
}

int helios_integer_test_setup(void **state) {
    garbagecollector *gc = helios_init_garbagecollector();
    helios_set_garbagecollector(gc);

    return 0;
}

int helios_integer_test_teardown(void **state) {
    helios_delete_garbagecollector(helios_get_garbagecollector());
    return 0;
}

void integer_test() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(helios_integer_init_test),
        cmocka_unit_test(helios_integer_from_cint_test),
    };
    cmocka_run_group_tests(tests, helios_integer_test_setup,
                           helios_integer_test_teardown);
}
