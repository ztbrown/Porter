#include <check.h>
#include <unistd.h>
#include <stdlib.h>

#include "receive_connections_test.h"

#include "../../src/http_utils.h"
#include "../../src/receive_connections.h"
#include "../../lib/mock.h"

SIMULACRUM(int, recv, 4, int, char*, int, int)
SIMULACRUM(int, getRequestType, 1, char*)
SIMULACRUM(void, handle_http_get, 2, char*, int)
SIMULACRUM(void, send_string, 2, char*, int)

static void setup()
{
    int success = 0;

    mock_set_return_value(&recv_mock, &success);
    mock_set_return_value(&getRequestType_mock, &success);
}

static void teardown()
{
    mock_reset_call_count(&recv_mock);
    mock_reset_call_count(&getRequestType_mock);
    mock_reset_call_count(&handle_http_get_mock);
    mock_reset_call_count(&send_string_mock);
}

struct send_string_args_s {
    char * msg;
    int connecting_socket;
};
struct send_string_args_s send_string_args;

void send_string_callback(char * msg, int socket)
{
    send_string_args.msg = malloc(strlen(msg) + 1);
    memcpy(send_string_args.msg, msg, strlen(msg) + 1);
    send_string_args.connecting_socket = socket;
}

START_TEST(it_receives_from_connecting_socket)
{
    // Arrange
    int connecting_socket = 0;

    // Act
    receive(connecting_socket);

    // Assert
    ck_assert_int_eq(mock_get_call_count(&recv_mock), 1);
}
END_TEST

START_TEST(it_handles_http_get)
{
    // Arrange
    int connecting_socket = 0;
    int request_type = 1;

    mock_set_return_value(&getRequestType_mock, &request_type);

    // Act
    receive(connecting_socket);

    // Assert
    ck_assert_int_eq(mock_get_call_count(&handle_http_get_mock), 1);

}
END_TEST

START_TEST(it_responds_to_POST_with_not_implemented)
{
    // Arrange
    int connecting_socket = 0;
    int request_type = 0;

    mock_set_return_value(&getRequestType_mock, &request_type);
    mock_set_callback(&send_string_mock, &send_string_callback);

    // Act
    receive(connecting_socket);

    // Assert
    ck_assert_int_eq(mock_get_call_count(&send_string_mock), 1);
    ck_assert_int_eq(send_string_args.connecting_socket, 0);
    ck_assert_str_eq(send_string_args.msg, "501 Not Implemented\n");

    // Clean up
    free(send_string_args.msg);
}
END_TEST

START_TEST(it_returns_bad_request_otherwise)
{
    // Arrange
    int connecting_socket = 0;
    int request_type = -1;

    mock_set_return_value(&getRequestType_mock, &request_type);
    mock_set_callback(&send_string_mock, &send_string_callback);

    // Act
    receive(connecting_socket);

    // Assert
    ck_assert_int_eq(mock_get_call_count(&send_string_mock), 1);
    ck_assert_int_eq(send_string_args.connecting_socket, 0);
    ck_assert_str_eq(send_string_args.msg, "400 Bad Request\n");
}
END_TEST

Suite *make_receive_connections_test_suite()
{
    Suite *s;
    TCase *tc;

    s = suite_create("Receive Connections");
    tc = tcase_create("Core");

    tcase_add_checked_fixture(tc, &setup, &teardown);

    tcase_add_test(tc, it_receives_from_connecting_socket);
    tcase_add_test(tc, it_handles_http_get);
    tcase_add_test(tc, it_responds_to_POST_with_not_implemented);
    tcase_add_test(tc, it_returns_bad_request_otherwise);

    suite_add_tcase(s, tc);

    return s;
}
