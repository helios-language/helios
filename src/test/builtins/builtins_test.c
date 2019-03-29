#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>

#include <builtins.h>
#include <builtins_test.h>

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

    };

    return integer_test() + float_test() + float_math_test() +
           integer_math_test() + integer_compare_test() + hashmap_test() +
           cmocka_run_group_tests_name("Builtins", tests,
                                       helios_builtin_test_setup,
                                       helios_builtin_test_teardown);
    ;
}