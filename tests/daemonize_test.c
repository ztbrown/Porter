#include <check.h>
#include <stdlib.h>
#include <stdio.h>

#include "../lib/mock.h"
#include "../src/daemonize.h"
#include "daemonize_test.h"

SIMULACRUM(int, getppid, 0)
SIMULACRUM(int, fork, 0)
SIMULACRUM(void, exit, 1, int)
SIMULACRUM(int, setsid, 0)
SIMULACRUM(int, chdir, 1, char*)

// Set up argument captor
int exit_arg_capture;
void exit_callback(int status)
{
    exit_arg_capture = status;
}

void setup()
{
    int defrtn = 0;
    mock_set_return_value(&getppid_mock, &defrtn);
    mock_set_return_value(&fork_mock, &defrtn);
    mock_set_return_value(&setsid_mock, &defrtn);
    mock_set_return_value(&chdir_mock, &defrtn);
    mock_set_return_value(&exit_mock, &defrtn);
}

void teardown()
{
    mock_reset_call_count(&fork_mock);
    mock_reset_call_count(&getppid_mock);
    mock_reset_call_count(&setsid_mock);
    mock_reset_call_count(&chdir_mock);
    mock_reset_call_count(&exit_mock);
}

START_TEST(daemonize_calls_fork_exactly_once)
{
    // Act
    daemonize();

    // Assert
    ck_assert_int_eq(mock_get_call_count(&fork_mock), 1);
}
END_TEST

START_TEST(it_does_not_fork_if_already_a_daemon)
{
    // Arrange
    int rtn = 1;

    mock_set_return_value(&getppid_mock, &rtn);
    mock_set_return_value(&setsid_mock, &rtn);
    mock_set_return_value(&chdir_mock, &rtn);

    // Act
    daemonize();

    // Assert
    ck_assert_int_eq(mock_get_call_count(&fork_mock), 0);
}
END_TEST

START_TEST(it_exits_with_failure_on_bad_PID)
{
    // Arrange
    int fork_rtn = -1;

    mock_set_return_value(&fork_mock, &fork_rtn);

    mock_set_callback(&exit_mock, &exit_callback);

    // Act
    daemonize();

    //Assert
    ck_assert_int_eq(mock_get_call_count(&exit_mock), 1);
    ck_assert_int_eq(exit_arg_capture, EXIT_FAILURE);

}
END_TEST

START_TEST(it_exits_with_success_on_good_PID)
{
    // Arrange
    int fork_rtn = 1;

    mock_set_return_value(&fork_mock, &fork_rtn);

    mock_set_callback(&exit_mock, &exit_callback);

    // Act
    daemonize();

    //Assert
    ck_assert_int_eq(mock_get_call_count(&exit_mock), 1);
    ck_assert_int_eq(exit_arg_capture, EXIT_SUCCESS);

}
END_TEST

START_TEST(it_sets_sid_for_child_process)
{
    // Act
    daemonize();

    // Assert
    ck_assert_int_eq(mock_get_call_count(&setsid_mock), 1);
}
END_TEST

START_TEST(it_exits_with_failure_if_setsid_fails)
{
    // Arrange
    int rtn = -1;

    mock_set_return_value(&setsid_mock, &rtn);

    // Act
    daemonize();

    // Assert
    ck_assert_int_eq(exit_arg_capture, EXIT_FAILURE);
}
END_TEST

START_TEST(it_changes_directory_to_prevent_cd_from_being_locked)
{
    // Arrange

    // Act
    daemonize();

    // Assert
    ck_assert_int_eq(mock_get_call_count(&chdir_mock), 1);
}
END_TEST

Suite *make_daemonize_test_suite(void)
{
    Suite *s;
    TCase *tc;

    s = suite_create("Daemonize");
    tc = tcase_create("Core");

    tcase_add_checked_fixture(tc, &setup, &teardown);

    tcase_add_test(tc, daemonize_calls_fork_exactly_once);
    tcase_add_test(tc, it_does_not_fork_if_already_a_daemon);
    tcase_add_test(tc, it_exits_with_failure_on_bad_PID);
    tcase_add_test(tc, it_exits_with_success_on_good_PID);
    tcase_add_test(tc, it_sets_sid_for_child_process);
    tcase_add_test(tc, it_exits_with_failure_if_setsid_fails);
    tcase_add_test(tc, it_changes_directory_to_prevent_cd_from_being_locked);

    suite_add_tcase(s, tc);

    return s;
}
