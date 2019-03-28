#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>

#include <builtins.h>

void helios_integer_init_test(void **state) {
    helios_object *obj = helios_integer_init();

    assert_true(IS_HELIOS_INTEGER(obj));
    assert_string_equal("Integer", obj->class->classname);

    helios_integer *intobj = TO_HELIOS_INTEGER(obj);
    assert_int_equal(intobj->value, HELIOS_INTEGER_DEFAULT_VALUE);
}

void helios_integer_tostring_pos_test(void **state) {
    helios_object *obj = helios_integer_from_cint(42);

    char *res = helios_string_to_charp(HELIOS_CALL_MEMBER(tostring, obj));
    assert_string_equal(res, "42");
    free(res);
}

void helios_integer_tostring_neg_test(void **state) {
    helios_object *obj = helios_integer_from_cint(-42);

    char *res = helios_string_to_charp(HELIOS_CALL_MEMBER(tostring, obj));
    assert_string_equal(res, "-42");
    free(res);
}

void helios_integer_tostring_large_test(void **state) {
    helios_object *obj = helios_integer_from_cint(2147483647);

    char *res = helios_string_to_charp(HELIOS_CALL_MEMBER(tostring, obj));
    assert_string_equal(res, "2147483647");
    free(res);
}

// alias of tostring so less extensive testing
void helios_integer_represent_test(void **state) {
    helios_object *obj = helios_integer_from_cint(42);

    char *res = helios_string_to_charp(HELIOS_CALL_MEMBER(represent, obj));
    assert_string_equal(res, "42");
    free(res);
}

void helios_integer_copy_test(void **state) {
    helios_object *obj1 = helios_integer_from_cint(42);
    helios_object *obj2 = HELIOS_CALL_MEMBER(copy, obj1);

    assert_ptr_not_equal(obj1, obj2);
    assert_int_equal(TO_HELIOS_INTEGER(obj1)->value,
                     TO_HELIOS_INTEGER(obj2)->value);
}

void helios_integer_from_cint_test(void **state) {
    helios_integer *obj = TO_HELIOS_INTEGER(helios_integer_from_cint(42));
    assert_int_equal(obj->value, 42);
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

int integer_test() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(helios_integer_init_test),
        cmocka_unit_test(helios_integer_from_cint_test),
        cmocka_unit_test(helios_integer_tostring_pos_test),
        cmocka_unit_test(helios_integer_tostring_neg_test),
        cmocka_unit_test(helios_integer_tostring_large_test),
        cmocka_unit_test(helios_integer_represent_test),
        cmocka_unit_test(helios_integer_copy_test),
    };
    return cmocka_run_group_tests_name("Helios integer", tests,
                                       helios_integer_test_setup,
                                       helios_integer_test_teardown);
}
