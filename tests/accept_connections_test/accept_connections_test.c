#include <check.h>
#include <signal.h>
#include <sys/wait.h>

#include "accept_connections_test.h"

#include "../../src/http_utils.h"
#include "../../src/accept_connections.h"
#include "../../lib/mock.h"

SIMULACRUM(int, accept, 3, int, struct sockaddr *, socklen_t *)
SIMULACRUM(void, close, 1, int)
SIMULACRUM(int, receive, 1, int)
SIMULACRUM(void, listen, 1, int)
SIMULACRUM(void, fork, 0)
SIMULACRUM(void, exit, 1, int)
SIMULACRUM(int, sigaction, 3, int, const struct sigaction *, struct sigaction *)
SIMULACRUM(int, waitpid, 3, pid_t, int *, int)

struct sigaction *captured_sigaction;
static void sigaction_callback(int signum, const struct sigaction *act, struct sigaction *oldact)
{
    act->sa_handler(0);
}

int waitpid_options;
static void waitpid_callback(pid_t pid, int *status, int options)
{
    waitpid_options = options;
}

static void setup()
{
    int success = 0;

    mock_set_return_value(&fork_mock, &success);
    mock_set_return_value(&sigaction_mock, &success);
}

static void teardown()
{
    mock_reset_call_count(&accept_mock);
    mock_reset_call_count(&close_mock);
    mock_reset_call_count(&listen_mock);
    mock_reset_call_count(&receive_mock);
    mock_reset_call_count(&exit_mock);
    mock_reset_call_count(&fork_mock);
    mock_reset_call_count(&sigaction_mock);
    mock_reset_call_count(&waitpid_mock);
}

START_TEST(it_calls_accept_and_sets_the_connecting_socket)
{
    // Arrange
    int current_socket = 0;
    int connecting_socket;
    socklen_t addr_size = 3;
    struct sockaddr_storage connector = {0};

    int mock_rtn_val = 80;

    mock_set_return_value(&accept_mock, &mock_rtn_val);

    // Act
    accept_connection(&current_socket, &connecting_socket, &addr_size, &connector);

    // Assert
    ck_assert_int_eq(mock_get_call_count(&accept_mock), 1);
    ck_assert_int_eq(connecting_socket, mock_rtn_val);
}
END_TEST

START_TEST(start_listener_calls_listen_with_current_socket_and_max_conn)
{
    // Arrange
    int current_socket = 0;

    // Act
    start_listener(current_socket);

    // Assert
    ck_assert_int_eq(mock_get_call_count(&listen_mock), 1);
}
END_TEST

START_TEST(it_handles_the_connecting_socket_and_calls_receive)
{
    // Arrange
    int current_socket = 0;
    int rtn = 1;
    mock_set_return_value(&receive_mock, &rtn);

    // Act
    handle_connection(current_socket);

    // Assert
    ck_assert_int_eq(mock_get_call_count(&receive_mock), 1);
}
END_TEST

START_TEST(handle_connection_forks_to_child_process_after_accepting_new_connection)
{
    // Arrange
    int connecting_socket = 0;

    // Act
    handle_connection(connecting_socket);

    // Assert
    ck_assert_int_eq(mock_get_call_count(&fork_mock), 1);
}
END_TEST

START_TEST(handle_connection_parent_process_closes_connecting_socket)
{
    // Arrange
    int connecting_socket = 0;
    int fork_rtn_parent = 1;
    mock_set_return_value(&fork_mock, &fork_rtn_parent);

    // Act
    handle_connection(connecting_socket);

    // Assert
    ck_assert_int_eq(mock_get_call_count(&fork_mock), 1);
    ck_assert_int_eq(mock_get_call_count(&close_mock), 1);
}
END_TEST

START_TEST(it_uses_sigaction_to_reap_zombie_child_processes)
{
    // Arrange
    int current_socket = 0;
    int fail = -1;

    mock_set_return_value(&waitpid_mock, &fail);
    mock_set_callback(&sigaction_mock, &sigaction_callback);
    mock_set_callback(&waitpid_mock, &waitpid_callback);

    // Act
    start_listener(current_socket);

    // Assert
    ck_assert_int_eq(mock_get_call_count(&sigaction_mock), 1);
    ck_assert_int_eq(mock_get_call_count(&waitpid_mock), 1);
    ck_assert_int_eq(waitpid_options, WNOHANG);
}
END_TEST

Suite *make_accept_connections_test_suite()
{
    Suite *s;
    TCase *tc;

    s = suite_create("Accept Connections");
    tc = tcase_create("Core");

    tcase_add_checked_fixture(tc, &setup, &teardown);

    tcase_add_test(tc, it_calls_accept_and_sets_the_connecting_socket);
    tcase_add_test(tc, start_listener_calls_listen_with_current_socket_and_max_conn);
    tcase_add_test(tc, it_handles_the_connecting_socket_and_calls_receive);
    tcase_add_test(tc, handle_connection_forks_to_child_process_after_accepting_new_connection);
    tcase_add_test(tc, it_uses_sigaction_to_reap_zombie_child_processes);

    suite_add_tcase(s, tc);

    return s;
}
