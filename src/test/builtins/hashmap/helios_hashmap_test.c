
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>

#include <builtins.h>

void helios_hashmap_init_test(void **state) {
    helios_object *obj = helios_hashmap_init();

    assert_true(IS_HELIOS_HASHMAP(obj));
    assert_string_equal("Hashmap", obj->class->classname);

    helios_hashmap *hmobj = TO_HELIOS_HASHMAP(obj);

    assert_int_equal(hmobj->size, HELIOS_HASHMAP_DEFAULT_SIZE);
    assert_int_equal(hmobj->filled, 0);
}

void helios_hashmap_put_test1() {
    helios_object *obj = helios_hashmap_init();
    helios_hashmap_put(obj, helios_integer_from_cint(10),
                       helios_integer_from_cint(11));
    helios_hashmap *hmobj = TO_HELIOS_HASHMAP(obj);

    uint32_t occurences = 0;
    for (uint32_t i = 0; i < hmobj->size; i++) {
        if (hmobj->value[i] != NULL && hmobj->value[i]->defunct == false) {
            helios_object *key = hmobj->value[i]->key;
            helios_object *value = hmobj->value[i]->value;
            assert_true(IS_HELIOS_INTEGER(key));
            assert_true(IS_HELIOS_INTEGER(value));
            assert_int_equal(helios_integer_to_cint(key), 10);
            assert_int_equal(helios_integer_to_cint(value), 11);
            occurences++;
        }
    }
    assert_int_equal(1, occurences);
}

void helios_hashmap_put_test2() {
    helios_object *obj = helios_hashmap_init();
    helios_hashmap_put(obj, helios_integer_from_cint(10),
                       helios_integer_from_cint(11));
    helios_hashmap_put(obj, helios_integer_from_cint(10),
                       helios_integer_from_cint(11));
    helios_hashmap *hmobj = TO_HELIOS_HASHMAP(obj);

    uint32_t occurences = 0;
    for (uint32_t i = 0; i < hmobj->size; i++) {
        if (hmobj->value[i] != NULL && hmobj->value[i]->defunct == false) {
            helios_object *key = hmobj->value[i]->key;
            helios_object *value = hmobj->value[i]->value;
            assert_true(IS_HELIOS_INTEGER(key));
            assert_true(IS_HELIOS_INTEGER(value));
            assert_int_equal(helios_integer_to_cint(key), 10);
            assert_int_equal(helios_integer_to_cint(value), 11);
            occurences++;
        }
    }
    assert_int_equal(2, occurences);
}

void helios_hashmap_rehash_test() {
    helios_object *obj = helios_hashmap_init();
    uint32_t amount = 100;

    for (uint32_t i = 0; i < amount; i++) {
        helios_hashmap_put(obj, helios_integer_from_cint(10),
                           helios_integer_from_cint(11));
    }

    helios_hashmap *hmobj = TO_HELIOS_HASHMAP(obj);

    uint32_t occurences = 0;
    for (uint32_t i = 0; i < hmobj->size; i++) {
        if (hmobj->value[i] != NULL && hmobj->value[i]->defunct == false) {
            helios_object *key = hmobj->value[i]->key;
            helios_object *value = hmobj->value[i]->value;
            assert_true(IS_HELIOS_INTEGER(key));
            assert_true(IS_HELIOS_INTEGER(value));
            assert_int_equal(helios_integer_to_cint(key), 10);
            assert_int_equal(helios_integer_to_cint(value), 11);
            occurences++;
        }
    }
    assert_int_equal(amount, occurences);
}

int helios_hashmap_test_setup(void **state) {
    garbagecollector *gc = helios_init_garbagecollector();
    helios_set_garbagecollector(gc);
    return 0;
}

int helios_hashmap_test_teardown(void **state) {
    helios_delete_garbagecollector(helios_get_garbagecollector());
    return 0;
}

int hashmap_test() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(helios_hashmap_init_test),
        cmocka_unit_test(helios_hashmap_put_test1),
        cmocka_unit_test(helios_hashmap_put_test2),
        cmocka_unit_test(helios_hashmap_rehash_test),

    };
    return cmocka_run_group_tests_name("Helios hashmap", tests,
                                       helios_hashmap_test_setup,
                                       helios_hashmap_test_teardown);
}
