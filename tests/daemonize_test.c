#include <check.h>
#include <stdlib.h>

#include "../lib/mock.h"
#include "../src/daemonize.h"
#include "daemonize_test.h"

SIMULACRUM(int, getppid, 0)
SIMULACRUM(void, fork, 0)

START_TEST(daemonize_calls_fork_exactly_once)
{
    int rtn = 0;

    mock_set_return_value(&getppid_mock, &rtn);

    daemonize();

    ck_assert_int_eq(mock_get_call_count(&fork_mock), 1);

    mock_off(&fork_mock);
    mock_off(&getppid_mock);
    mock_reset_call_count(&fork_mock);
    mock_reset_call_count(&getppid_mock);
}
END_TEST

START_TEST(it_does_not_fork_if_already_a_daemon)
{
    int rtn = 1;

    mock_on(&fork_mock);
    mock_on(&getppid_mock);

    mock_set_return_value(&getppid_mock, &rtn);

    ck_assert_int_eq(mock_get_call_count(&fork_mock), 0);

    mock_off(&fork_mock);
    mock_off(&getppid_mock);
    mock_reset_call_count(&fork_mock);
    mock_reset_call_count(&getppid_mock);
}
END_TEST

Suite *make_daemonize_test_suite(void)
{
    Suite *s;
    TCase *tc;

    s = suite_create("Daemonize");
    tc = tcase_create("Core");

    tcase_add_test(tc, daemonize_calls_fork_exactly_once);
    tcase_add_test(tc, it_does_not_fork_if_already_a_daemon);

    suite_add_tcase(s, tc);

    return s;
}
