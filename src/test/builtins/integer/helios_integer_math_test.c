#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include <cmocka.h>

#include <builtins.h>

#define FLOAT_DELTA 0.0001

void integer_add_float_test(void **state) {
    helios_object *a = helios_integer_from_cint(42);
    helios_object *b = helios_float_from_cdouble(42.42);

    helios_object *c = HELIOS_CALL_MEMBER(add, a, b);
    assert_true(IS_HELIOS_FLOAT(c));
    assert_true(TO_HELIOS_FLOAT(c)->value - 84.42 < FLOAT_DELTA);
}

void integer_add_null_test(void **state) {
    // cant be run yet as there is no helios exception handlers yet
    // helios_object *a = helios_integer_from_cint(42);
    // helios_object *b = NULL;

    // helios_object *c = HELIOS_CALL_MEMBER(add, a, b);
}

void integer_add_int_test(void **state) {
    helios_object *a = helios_integer_from_cint(42);
    helios_object *b = helios_integer_from_cint(42);

    helios_object *c = HELIOS_CALL_MEMBER(add, a, b);
    assert_true(IS_HELIOS_INTEGER(c));
    assert_true(TO_HELIOS_INTEGER(c)->value == 84);
}

void integer_add_string_test(void **state) {
    // cant be run yet as there is no helios exception handlers yet
    // helios_object *a = helios_integer_from_cint(42);
    // helios_object *b = helios_string_from_charp("yeet");

    // helios_object *c = HELIOS_CALL_MEMBER(add, a, b);
}

void integer_subtract_float_test(void **state) {
    helios_object *a = helios_integer_from_cint(42);
    helios_object *b = helios_float_from_cdouble(42.42);

    helios_object *c = HELIOS_CALL_MEMBER(subtract, a, b);
    assert_true(IS_HELIOS_FLOAT(c));
    assert_true(TO_HELIOS_FLOAT(c)->value - -0.42 < FLOAT_DELTA);
}

void integer_subtract_null_test(void **state) {
    // cant be run yet as there is no helios exception handlers yet
    // helios_object *a = helios_integer_from_cint(42);
    // helios_object *b = NULL;

    // helios_object *c = HELIOS_CALL_MEMBER(subtract, a, b);
}

void integer_subtract_int_test(void **state) {
    helios_object *a = helios_integer_from_cint(42);
    helios_object *b = helios_integer_from_cint(42);

    helios_object *c = HELIOS_CALL_MEMBER(subtract, a, b);
    assert_true(IS_HELIOS_INTEGER(c));
    assert_true(TO_HELIOS_INTEGER(c)->value == 0);
}

void integer_subtract_string_test(void **state) {
    // cant be run yet as there is no helios exception handlers yet
    // helios_object *a = helios_integer_from_cint(42);
    // helios_object *b = helios_string_from_charp("yeet");

    // helios_object *c = HELIOS_CALL_MEMBER(subtract, a, b);
}

void integer_multiply_float_test(void **state) {
    helios_object *a = helios_integer_from_cint(42);
    helios_object *b = helios_float_from_cdouble(42.42);

    helios_object *c = HELIOS_CALL_MEMBER(multiply, a, b);
    assert_true(IS_HELIOS_FLOAT(c));
    assert_true(TO_HELIOS_FLOAT(c)->value - 1781.64 < FLOAT_DELTA);
}

void integer_multiply_null_test(void **state) {
    // cant be run yet as there is no helios exception handlers yet
    // helios_object *a = helios_integer_from_cint(42);
    // helios_object *b = NULL;

    // helios_object *c = HELIOS_CALL_MEMBER(multiply, a, b);
}

void integer_multiply_int_test(void **state) {
    helios_object *a = helios_integer_from_cint(42);
    helios_object *b = helios_integer_from_cint(42);

    helios_object *c = HELIOS_CALL_MEMBER(multiply, a, b);
    assert_true(IS_HELIOS_INTEGER(c));
    assert_true(TO_HELIOS_INTEGER(c)->value == 1764);
}

void integer_multiply_string_test(void **state) {
    // cant be run yet as there is no helios exception handlers yet
    // helios_object *a = helios_integer_from_cint(42);
    // helios_object *b = helios_string_from_charp("yeet");

    // helios_object *c = HELIOS_CALL_MEMBER(multiply, a, b);
}

void integer_divide_float_test(void **state) {
    helios_object *a = helios_integer_from_cint(42);
    helios_object *b = helios_float_from_cdouble(42.42);

    helios_object *c = HELIOS_CALL_MEMBER(divide, a, b);
    assert_true(IS_HELIOS_FLOAT(c));
    assert_true(TO_HELIOS_FLOAT(c)->value - 0.9900990099 < FLOAT_DELTA);
}

void integer_divide_null_test(void **state) {
    // cant be run yet as there is no helios exception handlers yet
    // helios_object *a = helios_integer_from_cint(42);
    // helios_object *b = NULL;

    // helios_object *c = HELIOS_CALL_MEMBER(divide, a, b);
}

void integer_divide_int_test(void **state) {
    helios_object *a = helios_integer_from_cint(42);
    helios_object *b = helios_integer_from_cint(42);

    helios_object *c = HELIOS_CALL_MEMBER(divide, a, b);
    assert_true(IS_HELIOS_FLOAT(c));
    assert_true(TO_HELIOS_FLOAT(c)->value - 1 < FLOAT_DELTA);
}

void integer_divide_string_test(void **state) {
    // cant be run yet as there is no helios exception handlers yet
    // helios_object *a = helios_integer_from_cint(42);
    // helios_object *b = helios_string_from_charp("yeet");

    // helios_object *c = HELIOS_CALL_MEMBER(divide, a, b);
}

int helios_integer_math_test_setup(void **state) {
    garbagecollector *gc = helios_init_garbagecollector();
    helios_set_garbagecollector(gc);
    return 0;
}

int helios_integer_math_test_teardown(void **state) {
    helios_delete_garbagecollector(helios_get_garbagecollector());
    return 0;
}

int integer_math_test() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(integer_add_float_test),
        cmocka_unit_test(integer_add_null_test),
        cmocka_unit_test(integer_add_int_test),
        cmocka_unit_test(integer_add_string_test),

        cmocka_unit_test(integer_subtract_float_test),
        cmocka_unit_test(integer_subtract_null_test),
        cmocka_unit_test(integer_subtract_int_test),
        cmocka_unit_test(integer_subtract_string_test),

        cmocka_unit_test(integer_multiply_float_test),
        cmocka_unit_test(integer_multiply_null_test),
        cmocka_unit_test(integer_multiply_int_test),
        cmocka_unit_test(integer_multiply_string_test),

        cmocka_unit_test(integer_divide_float_test),
        cmocka_unit_test(integer_divide_null_test),
        cmocka_unit_test(integer_divide_int_test),
        cmocka_unit_test(integer_divide_string_test),
    };
    return cmocka_run_group_tests_name("Helios integer math", tests,
                                       helios_integer_math_test_setup,
                                       helios_integer_math_test_teardown);
}
