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

// clang-format on

Suite *dysk_c_suite(void) {
    Suite *s = suite_create("Dysk C");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_starts_with);

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
