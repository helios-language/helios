#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>

#include <builtins.h>

void helios_float_init_test(void **state) {
    helios_object *obj = helios_float_init();

    assert_true(IS_HELIOS_FLOAT(obj));
    assert_string_equal("Float", obj->class->classname);

    helios_float *intobj = TO_HELIOS_FLOAT(obj);
    assert_true(intobj->value == HELIOS_FLOAT_DEFAULT_VALUE);
}

void helios_float_tostring_pos_test(void **state) {
    helios_object *obj = helios_float_from_cdouble(42.42);

    char *res = helios_string_to_charp(HELIOS_CALL_MEMBER(tostring, obj));
    assert_string_equal(res, "42.420000");
    free(res);
}

void helios_float_tostring_neg_test(void **state) {
    helios_object *obj = helios_float_from_cdouble(-42.42);

    char *res = helios_string_to_charp(HELIOS_CALL_MEMBER(tostring, obj));
    assert_string_equal(res, "-42.420000");
    free(res);
}

void helios_float_tostring_large_test(void **state) {
    helios_object *obj =
        helios_float_from_cdouble(3.14159265358979323846264338327950);

    char *res = helios_string_to_charp(HELIOS_CALL_MEMBER(tostring, obj));
    assert_string_equal(res, "3.141593");
    free(res);
}

// alias of tostring so less extensive testing
void helios_float_represent_test(void **state) {
    helios_object *obj = helios_float_from_cdouble(42.42);

    char *res = helios_string_to_charp(HELIOS_CALL_MEMBER(represent, obj));
    assert_string_equal(res, "42.420000");
    free(res);
}

void helios_float_copy_test(void **state) {
    helios_object *obj1 = helios_float_from_cdouble(42.0);
    helios_object *obj2 = HELIOS_CALL_MEMBER(copy, obj1);

    assert_ptr_not_equal(obj1, obj2);
    assert_true(TO_HELIOS_FLOAT(obj1)->value == TO_HELIOS_FLOAT(obj2)->value);
}

void helios_float_from_cdouble_test(void **state) {
    helios_float *obj = TO_HELIOS_FLOAT(helios_float_from_cdouble(42.0));
    assert_int_equal(obj->value, 42.0);
}

void helios_float_hash_equal_test(void **state) {
    helios_integer *obj1 =
        TO_HELIOS_INTEGER(helios_float_hash(helios_float_from_cdouble(42.0)));
    helios_integer *obj2 =
        TO_HELIOS_INTEGER(helios_float_hash(helios_float_from_cdouble(42.0)));
    assert_int_equal(obj1->value, obj2->value);
}

void helios_float_hash_not_equal_test(void **state) {
    helios_integer *obj1 =
        TO_HELIOS_INTEGER(helios_float_hash(helios_float_from_cdouble(42.0)));
    helios_integer *obj2 =
        TO_HELIOS_INTEGER(helios_float_hash(helios_float_from_cdouble(10)));
    assert_false(obj1->value == obj2->value);
}

void helios_float_hash_close_test(void **state) {
    helios_integer *obj1 = TO_HELIOS_INTEGER(
        helios_float_hash(helios_float_from_cdouble(42.00000000000009)));
    helios_integer *obj2 = TO_HELIOS_INTEGER(
        helios_float_hash(helios_float_from_cdouble(42.00000000000001)));
    assert_int_equal(obj1->value, obj2->value);
}

int helios_float_test_setup(void **state) {
    garbagecollector *gc = helios_init_garbagecollector();
    helios_set_garbagecollector(gc);
    return 0;
}

int helios_float_test_teardown(void **state) {
    helios_delete_garbagecollector(helios_get_garbagecollector());
    return 0;
}

int float_test() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(helios_float_init_test),
        cmocka_unit_test(helios_float_from_cdouble_test),
        cmocka_unit_test(helios_float_tostring_pos_test),
        cmocka_unit_test(helios_float_tostring_neg_test),
        cmocka_unit_test(helios_float_tostring_large_test),
        cmocka_unit_test(helios_float_represent_test),
        cmocka_unit_test(helios_float_copy_test),
        cmocka_unit_test(helios_float_hash_equal_test),
        cmocka_unit_test(helios_float_hash_not_equal_test),
        cmocka_unit_test(helios_float_hash_close_test),
    };
    return cmocka_run_group_tests_name("Helios float", tests,
                                       helios_float_test_setup,
                                       helios_float_test_teardown);
}
