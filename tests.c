#include <check.h>
#include <stdlib.h>

#include "src/chstring.h"

// clang-format off

START_TEST(test_starts_with)
{
    ck_assert(starts_with("/dev/mapper/root", "/dev"));
    ck_assert(!starts_with("/dev/mapper/root", "dev"));
}
END_TEST

START_TEST(test_string_width)
{
    ck_assert(string_width("hello") == 5);
    ck_assert(string_width("なかの あずさ") == 13);
    ck_assert(string_width("Zażółć gęślą żółć") == 17);
}
END_TEST

START_TEST(test_string_truncate)
{
    string test_str = NULL;
    bool truncated = false;

    test_str = strdup("Hello, World!");
    truncated = string_truncate(test_str, 5);
    ck_assert(truncated);
    ck_assert_str_eq(test_str, "Hello");
    free_string(&test_str);

    test_str = strdup("Hello, World!");
    truncated = string_truncate(test_str, 128);
    ck_assert(!truncated);
    ck_assert_str_eq(test_str, "Hello, World!");
    free_string(&test_str);

    test_str = strdup("なかの あずさ");
    truncated = string_truncate(test_str, 5);
    ck_assert(truncated);
    ck_assert_str_eq(test_str, "なか");
    free_string(&test_str);
}
END_TEST

START_TEST(test_parse_double)
{
    double val = 0.0;
    bool ok = false;

    ok = ch_parse_double("0.025", &val);
    ck_assert(ok);
    ck_assert_double_eq(val, 0.025);

    ok = ch_parse_double("invalid", &val);
    ck_assert(!ok);
}
END_TEST

START_TEST(test_parse_int)
{
    int val = 0;
    bool ok = false;

    ok = ch_parse_int("42", &val);
    ck_assert(ok);
    ck_assert_int_eq(val, 42);

    ok = ch_parse_int("invalid", &val);
    ck_assert(!ok);

    ok = ch_parse_int("10m", &val);
    ck_assert(!ok);

    ok = ch_parse_int("0.025", &val);
    ck_assert(!ok);
}
END_TEST

// clang-format on

Suite *dysk_c_suite(void) {
    Suite *s = suite_create("chstring");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_starts_with);
    tcase_add_test(tc_core, test_string_width);
    tcase_add_test(tc_core, test_string_truncate);
    tcase_add_test(tc_core, test_parse_int);
    tcase_add_test(tc_core, test_parse_double);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    Suite *s = dysk_c_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    int number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
