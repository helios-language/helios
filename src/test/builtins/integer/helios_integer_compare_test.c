#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include <cmocka.h>

#include <builtins.h>

void integers_integer_equal_test_true(void **state) {
    helios_object *res = helios_integer_equal(helios_integer_from_cint(42),
                                              helios_integer_from_cint(42));

    assert_true(IS_HELIOS_BOOLEAN(res));
    assert_true(HELIOS_BOOLEAN_IS_TRUE(res));
}

void integers_integer_equal_test_false(void **state) {
    helios_object *res = helios_integer_equal(helios_integer_from_cint(42),
                                              helios_integer_from_cint(11));

    assert_true(IS_HELIOS_BOOLEAN(res));
    assert_true(HELIOS_BOOLEAN_IS_FALSE(res));
}

void integers_integer_notequal_test_true(void **state) {
    helios_object *res = helios_integer_notequal(helios_integer_from_cint(42),
                                                 helios_integer_from_cint(42));

    assert_true(IS_HELIOS_BOOLEAN(res));
    assert_true(HELIOS_BOOLEAN_IS_FALSE(res));
}

void integers_integer_notequal_test_false(void **state) {
    helios_object *res = helios_integer_notequal(helios_integer_from_cint(42),
                                                 helios_integer_from_cint(11));

    assert_true(IS_HELIOS_BOOLEAN(res));
    assert_true(HELIOS_BOOLEAN_IS_TRUE(res));
}

void integers_float_equal_test_true(void **state) {
    helios_object *res = helios_integer_equal(helios_integer_from_cint(42),
                                              helios_float_from_cdouble(42.0));

    assert_true(IS_HELIOS_BOOLEAN(res));
    assert_true(HELIOS_BOOLEAN_IS_TRUE(res));
}

void integers_float_equal_test_false(void **state) {
    helios_object *res = helios_integer_equal(
        helios_integer_from_cint(42), helios_float_from_cdouble(42.00001));

    assert_true(IS_HELIOS_BOOLEAN(res));
    assert_true(HELIOS_BOOLEAN_IS_FALSE(res));
}

void integers_float_notequal_test_true(void **state) {
    helios_object *res = helios_integer_notequal(
        helios_integer_from_cint(42), helios_float_from_cdouble(42.0));

    assert_true(IS_HELIOS_BOOLEAN(res));
    assert_true(HELIOS_BOOLEAN_IS_FALSE(res));
}

void integers_float_notequal_test_false(void **state) {
    helios_object *res = helios_integer_notequal(
        helios_integer_from_cint(42), helios_float_from_cdouble(42.00001));

    assert_true(IS_HELIOS_BOOLEAN(res));
    assert_true(HELIOS_BOOLEAN_IS_TRUE(res));
}

void integers_boolean_true_test(void **state) {
    helios_object *res = helios_integer_boolean(helios_integer_from_cint(42));

    assert_true(IS_HELIOS_BOOLEAN(res));
    assert_true(HELIOS_BOOLEAN_IS_TRUE(res));
}

void integers_boolean_false_test(void **state) {
    helios_object *res = helios_integer_boolean(helios_integer_from_cint(0));

    assert_true(IS_HELIOS_BOOLEAN(res));
    assert_true(HELIOS_BOOLEAN_IS_FALSE(res));
}

void integers_boolean_equal_test_true(void **state) {
    helios_object *res = helios_integer_equal(helios_integer_from_cint(1),
                                              helios_boolean_from_cbool(true));

    assert_true(IS_HELIOS_BOOLEAN(res));
    assert_true(HELIOS_BOOLEAN_IS_TRUE(res));
}

void integers_boolean_equal_test_false(void **state) {
    helios_object *res = helios_integer_equal(helios_integer_from_cint(1),
                                              helios_boolean_from_cbool(false));

    assert_true(IS_HELIOS_BOOLEAN(res));
    assert_true(HELIOS_BOOLEAN_IS_FALSE(res));
}

void integers_boolean_notequal_test_true(void **state) {
    helios_object *res = helios_integer_notequal(
        helios_integer_from_cint(1), helios_boolean_from_cbool(true));

    assert_true(IS_HELIOS_BOOLEAN(res));
    assert_true(HELIOS_BOOLEAN_IS_FALSE(res));
}

void integers_boolean_notequal_test_false(void **state) {
    helios_object *res = helios_integer_notequal(
        helios_integer_from_cint(42), helios_boolean_from_cbool(true));

    assert_true(IS_HELIOS_BOOLEAN(res));
    assert_true(HELIOS_BOOLEAN_IS_TRUE(res));
}

int helios_integer_compare_test_setup(void **state) {
    garbagecollector *gc = helios_init_garbagecollector();
    helios_set_garbagecollector(gc);
    return 0;
}

int helios_integer_compare_test_teardown(void **state) {
    helios_delete_garbagecollector(helios_get_garbagecollector());
    return 0;
}

int integer_compare_test() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(integers_integer_equal_test_true),
        cmocka_unit_test(integers_integer_equal_test_false),
        cmocka_unit_test(integers_integer_notequal_test_true),
        cmocka_unit_test(integers_integer_notequal_test_false),

        cmocka_unit_test(integers_float_equal_test_true),
        cmocka_unit_test(integers_float_equal_test_false),
        cmocka_unit_test(integers_float_notequal_test_true),
        cmocka_unit_test(integers_float_notequal_test_false),

        cmocka_unit_test(integers_boolean_equal_test_true),
        cmocka_unit_test(integers_boolean_equal_test_false),
        cmocka_unit_test(integers_boolean_notequal_test_true),
        cmocka_unit_test(integers_boolean_notequal_test_false),

        cmocka_unit_test(integers_boolean_true_test),
        cmocka_unit_test(integers_boolean_false_test),
    };
    return cmocka_run_group_tests_name("Helios integer compare", tests,
                                       helios_integer_compare_test_setup,
                                       helios_integer_compare_test_teardown);
}
