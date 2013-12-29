#include <check.h>
#include <stdint.h>
#include <canutil/read.h>

const uint64_t BIG_ENDIAN_TEST_DATA = __builtin_bswap64(0xEB00000000000000);

START_TEST (test_parse_float)
{
    float result = bitfield_parse_float(BIG_ENDIAN_TEST_DATA, 2, 4, 1001.0, -30000.0);
    float correctResult = 0xA * 1001.0 - 30000.0;
    fail_unless(result == correctResult,
            "parse is incorrect: %f but should be %f", result, correctResult);
}
END_TEST

Suite* canreadSuite(void) {
    Suite* s = suite_create("read");
    TCase *tc_core = tcase_create("core");
    tcase_add_checked_fixture(tc_core, NULL, NULL);
    tcase_add_test(tc_core, test_parse_float);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int numberFailed;
    Suite* s = canreadSuite();
    SRunner *sr = srunner_create(s);
    // Don't fork so we can actually use gdb
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);
    numberFailed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (numberFailed == 0) ? 0 : 1;
}
