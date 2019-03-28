
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#include <cmocka.h>

#include <builtins.h>
#include <parser.h>

void parser_preprocess_donothing_test(void **state) {
    char *input = "yeet = 42";
    char *output = parser_preprocess(input);
    assert_string_equal(input, output);
    free(output);
}

void parser_preprocess_removecomment_test(void **state) {
    char *input = "yeet = 42 # yeet";
    char *output = parser_preprocess(input);
    assert_string_equal("yeet = 42 ", output);
    free(output);
}

void parser_preprocess_removecomment_multiline_test(void **state) {
    char *input = "yeet = 42 # yeet\nyeet = 42";
    char *output = parser_preprocess(input);
    assert_string_equal("yeet = 42 \nyeet = 42", output);
    free(output);
}


void parser_preprocess_carriage_remove_test(void **state) {
    char *input = "yeet = 42\r\n";
    char *output = parser_preprocess(input);
    assert_string_equal("yeet = 42\n", output);
    free(output);
}

void parser_new_test(void **state) {
    char *teststr = "yeet";
    Parser_t *parser = parser_new(teststr);

    assert_int_equal(parser->line, 1);
    assert_int_equal(parser->character, 0);

    assert_int_equal(parser->prev, '\0');
    assert_int_equal(parser->curr, '\0');
    assert_int_equal(parser->next, '\0');

    assert_string_equal(parser->code, teststr);
    assert_int_equal(parser->codelength, strlen(teststr));

    assert_int_equal(parser->index, 0);

    parser_free(parser);
}

void parser_advance_test(void **state) {
    char *teststr = "ye\net";
    Parser_t *parser = parser_new(teststr);

    /////////////////

    parser_advance(parser);

    assert_int_equal(parser->line, 1);
    assert_int_equal(parser->character, 1);

    assert_int_equal(parser->prev, '\0');
    assert_int_equal(parser->curr, '\0');
    assert_int_equal(parser->next, 'y');

    assert_string_equal(parser->code, teststr);
    assert_int_equal(parser->codelength, strlen(teststr));

    assert_int_equal(parser->index, 1);

    /////////////////

    parser_advance(parser);

    assert_int_equal(parser->line, 1);
    assert_int_equal(parser->character, 2);

    assert_int_equal(parser->prev, '\0');
    assert_int_equal(parser->curr, 'y');
    assert_int_equal(parser->next, 'e');

    assert_string_equal(parser->code, teststr);
    assert_int_equal(parser->codelength, strlen(teststr));

    assert_int_equal(parser->index, 2);

    /////////////////

    parser_advance(parser);

    assert_int_equal(parser->line, 1);
    assert_int_equal(parser->character, 3);

    assert_int_equal(parser->prev, 'y');
    assert_int_equal(parser->curr, 'e');
    assert_int_equal(parser->next, '\n');

    assert_string_equal(parser->code, teststr);
    assert_int_equal(parser->codelength, strlen(teststr));

    assert_int_equal(parser->index, 3);

    /////////////////

    parser_advance(parser);

    assert_int_equal(parser->line, 2);
    assert_int_equal(parser->character, 0);

    assert_int_equal(parser->prev, 'e');
    assert_int_equal(parser->curr, '\n');
    assert_int_equal(parser->next, 'e');

    assert_string_equal(parser->code, teststr);
    assert_int_equal(parser->codelength, strlen(teststr));

    assert_int_equal(parser->index, 4);

    parser_free(parser);
}

void parser_acceptchar_test(void **state) {
    // char *teststr = "yeet";
    // Parser_t *parser = parser_new(teststr);

    // assert_false(parser_acceptchar(parser, 'e'));
    // assert_true(parser_acceptchar(parser, 'y'));
    // assert_true(parser_acceptchar(parser, 'e'));
    // assert_true(parser_acceptchar(parser, 'e'));
    // assert_false(parser_acceptchar(parser, 'e'));
    // assert_true(parser_acceptchar(parser, 't'));

    // parser_free(parser);
}

void parser_acceptchar_accepted_test(void **state) {
    // char *teststr = "yeet";
    // Parser_t *parser = parser_new(teststr);

    // assert_true(parser_acceptchar(parser, 'y'));
    // assert_int_equal(parser->accepted[0], 'y');

    // parser_free(parser);
}

void parser_acceptstring_test(void **state) {
    // char *teststr = "yeet";
    // Parser_t *parser = parser_new(teststr);

    // assert_false(parser_acceptstring(parser, 'yeep'));
    // assert_true(parser_acceptstring(parser, 'yeet'));

    // parser_free(parser);
}

void parser_acceptstring_accepted_test(void **state) {
    // char *teststr = "yeet";
    // Parser_t *parser = parser_new(teststr);

    // assert_false(parser_acceptstring(parser, 'yeep'));
    // assert_true(parser_acceptstring(parser, 'yeet'));

    // parser_free(parser);
}

int helios_parserutil_test_setup(void **state) {
    garbagecollector *gc = helios_init_garbagecollector();
    helios_set_garbagecollector(gc);
    return 0;
}

int helios_parserutil_test_teardown(void **state) {
    helios_delete_garbagecollector(helios_get_garbagecollector());
    return 0;
}

int parserutil_test() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(parser_preprocess_donothing_test),
        cmocka_unit_test(parser_preprocess_removecomment_test),
        cmocka_unit_test(parser_preprocess_removecomment_multiline_test),
        cmocka_unit_test(parser_preprocess_carriage_remove_test),
        cmocka_unit_test(parser_new_test),
        cmocka_unit_test(parser_advance_test),
        cmocka_unit_test(parser_acceptchar_test),
        cmocka_unit_test(parser_acceptchar_accepted_test),
        cmocka_unit_test(parser_acceptstring_test),
        cmocka_unit_test(parser_acceptstring_accepted_test),
    };

    return cmocka_run_group_tests_name("Parserutil", tests,
                                       helios_parserutil_test_setup,
                                       helios_parserutil_test_teardown);
}
