#include <check.h>

#include "tests.h"

int main(void) {
    SRunner *sr = srunner_create(start_test_suite());

    srunner_add_suite(sr, moving_test_suite());
    srunner_add_suite(sr, attaching_test_suite());
    srunner_add_suite(sr, misc_test_suite());
    srunner_add_suite(sr, library_interface_test_suite());

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_VERBOSE);
    int failed = srunner_ntests_failed(sr) != 0;
    srunner_free(sr);

    return failed;
}
