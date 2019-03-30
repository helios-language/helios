#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>

#include <builtins.h>
#include <builtins_test.h>

helios_object *booleanfunc1(helios_object *obj) {
    fail_msg("if this is called the operator isnt lazy");
    return NULL;
}

void builtins_helios_lazy_or1(void **state) {
    assert_true(HELIOS_BOOLEAN_IS_FALSE(
        helios_lazy_or(helios_boolean_false, helios_boolean_false)));
}

void builtins_helios_lazy_or2(void **state) {
    assert_true(HELIOS_BOOLEAN_IS_TRUE(
        helios_lazy_or(helios_boolean_true, helios_boolean_false)));
}

void builtins_helios_lazy_or3(void **state) {
    assert_true(HELIOS_BOOLEAN_IS_TRUE(
        helios_lazy_or(helios_boolean_true, helios_boolean_true)));
}

void builtins_helios_lazy_or4(void **state) {
    assert_true(HELIOS_BOOLEAN_IS_TRUE(helios_lazy_or(
        helios_integer_from_cint(42), helios_integer_from_cint(42))));
}

void builtins_helios_lazy_or5(void **state) {
    assert_true(HELIOS_BOOLEAN_IS_TRUE(helios_lazy_or(
        helios_integer_from_cint(0), helios_integer_from_cint(42))));
}

void builtins_helios_lazy_or6(void **state) {
    helios_type helios_test_type = {
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_object_type),
        HELIOS_OBJECT_COMPARISON_FIELDS_BOOLEAN(booleanfunc1),
    };
    helios_object myobject = (helios_object){
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_test_type),
    };
    assert_true(HELIOS_BOOLEAN_IS_TRUE(
        helios_lazy_or(helios_integer_from_cint(1), &myobject)));
}

void builtins_helios_lazy_and1(void **state) {
    assert_true(HELIOS_BOOLEAN_IS_FALSE(
        helios_lazy_and(helios_boolean_false, helios_boolean_false)));
}

void builtins_helios_lazy_and2(void **state) {
    assert_true(HELIOS_BOOLEAN_IS_FALSE(
        helios_lazy_and(helios_boolean_true, helios_boolean_false)));
}

void builtins_helios_lazy_and3(void **state) {
    assert_true(HELIOS_BOOLEAN_IS_TRUE(
        helios_lazy_and(helios_boolean_true, helios_boolean_true)));
}

void builtins_helios_lazy_and4(void **state) {
    assert_true(HELIOS_BOOLEAN_IS_TRUE(helios_lazy_and(
        helios_integer_from_cint(42), helios_integer_from_cint(42))));
}

void builtins_helios_lazy_and5(void **state) {
    assert_true(HELIOS_BOOLEAN_IS_FALSE(helios_lazy_and(
        helios_integer_from_cint(0), helios_integer_from_cint(42))));
}

void builtins_helios_lazy_and6(void **state) {
    helios_type helios_test_type = {
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_object_type),
        HELIOS_OBJECT_COMPARISON_FIELDS_BOOLEAN(booleanfunc1),
    };
    helios_object myobject = (helios_object){
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_test_type),
    };
    assert_true(HELIOS_BOOLEAN_IS_FALSE(
        helios_lazy_and(helios_integer_from_cint(0), &myobject)));
}

void builtins_helios_not_true(void **state) {
    assert_true(HELIOS_BOOLEAN_IS_TRUE(helios_not(helios_boolean_false)));
}

void builtins_helios_not_false(void **state) {
    assert_true(HELIOS_BOOLEAN_IS_FALSE(helios_not(helios_boolean_true)));
}

void builtins_helios_hash_nofunc_test(void **state) {
    helios_type helios_test_type = {
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_object_type),
    };
    helios_object myobject = (helios_object){
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_test_type),
    };

    assert_int_equal(helios_integer_to_cint(helios_hash(&myobject)), &myobject);
}

void builtins_helios_hash_same_nofunc_test(void **state) {
    helios_type helios_test_type = {
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_object_type),
    };
    helios_object myobject = (helios_object){
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_test_type),
    };

    assert_int_equal(helios_integer_to_cint(helios_hash(&myobject)),
                     helios_integer_to_cint(helios_hash(&myobject)));
}

helios_object *hashfunc1(helios_object *obj) {
    return helios_integer_from_cint(42);
}

void builtins_helios_hash_test(void **state) {
    helios_type helios_test_type = {
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_object_type),
        HELIOS_OBJECT_BASIC_FIELDS_HASH(hashfunc1),
    };
    helios_object myobject = (helios_object){
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_test_type),
    };
    assert_int_equal(helios_integer_to_cint(helios_hash(&myobject)), 42);
}

void builtins_helios_hash_different_nofunc_test(void **state) {
    helios_type helios_test_type1 = {
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_object_type),
    };

    helios_type helios_test_type2 = {
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_object_type),
    };
    helios_object myobject1 = (helios_object){
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_test_type1),
    };
    helios_object myobject2 = (helios_object){
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_test_type2),
    };
    assert_false(helios_integer_to_cint(helios_hash(&myobject1)) ==
                 helios_integer_to_cint(helios_hash(&myobject2)));
}

helios_object *hashfunc2(helios_object *obj) {
    return helios_integer_from_cint(43);
}

void builtins_helios_hash_different_test(void **state) {
    helios_type helios_test_type1 = {
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_object_type),
        HELIOS_OBJECT_BASIC_FIELDS_HASH(hashfunc1),
    };

    helios_type helios_test_type2 = {
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_object_type),
        HELIOS_OBJECT_BASIC_FIELDS_HASH(hashfunc2),
    };
    helios_object myobject1 = (helios_object){
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_test_type1),
    };
    helios_object myobject2 = (helios_object){
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_test_type2),
    };
    assert_false(helios_integer_to_cint(helios_hash(&myobject1)) ==
                 helios_integer_to_cint(helios_hash(&myobject2)));
}

int helios_builtin_test_setup(void **state) {
    garbagecollector *gc = helios_init_garbagecollector();
    helios_set_garbagecollector(gc);
    return 0;
}

int helios_builtin_test_teardown(void **state) {
    helios_delete_garbagecollector(helios_get_garbagecollector());
    return 0;
}

int builtins_test() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(builtins_helios_hash_nofunc_test),
        cmocka_unit_test(builtins_helios_hash_same_nofunc_test),
        cmocka_unit_test(builtins_helios_hash_test),
        cmocka_unit_test(builtins_helios_hash_different_nofunc_test),
        cmocka_unit_test(builtins_helios_hash_different_test),
        cmocka_unit_test(builtins_helios_not_true),
        cmocka_unit_test(builtins_helios_not_false),
        cmocka_unit_test(builtins_helios_lazy_and1),
        cmocka_unit_test(builtins_helios_lazy_and2),
        cmocka_unit_test(builtins_helios_lazy_and3),
        cmocka_unit_test(builtins_helios_lazy_and4),
        cmocka_unit_test(builtins_helios_lazy_and5),
        cmocka_unit_test(builtins_helios_lazy_and6),
        cmocka_unit_test(builtins_helios_lazy_or1),
        cmocka_unit_test(builtins_helios_lazy_or2),
        cmocka_unit_test(builtins_helios_lazy_or3),
        cmocka_unit_test(builtins_helios_lazy_or4),
        cmocka_unit_test(builtins_helios_lazy_or5),
        cmocka_unit_test(builtins_helios_lazy_or6),

    };

    return integer_test() + float_test() + float_math_test() +
           integer_math_test() + integer_compare_test() + hashmap_test() +
           cmocka_run_group_tests_name("Builtins", tests,
                                       helios_builtin_test_setup,
                                       helios_builtin_test_teardown);
    ;
}