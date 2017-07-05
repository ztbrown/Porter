#include <check.h>
#include <stdlib.h>
#include <stdio.h>

#include "../lib/mock.h"
#include "../src/create_socket.h"
#include "create_socket_test.h"

SIMULACRUM(int, socket, 3, int, int, int)
SIMULACRUM(void, perror, 1, char*)

static void setup()
{
    int defrtn = 0;
    mock_set_return_value(&socket_mock, &defrtn);
    mock_set_return_value(&perror_mock, &defrtn);
}

static void teardown()
{
   mock_reset_call_count(&socket_mock);
   mock_reset_call_count(&perror_mock);
}

START_TEST(it_creates_a_socket)
{
    // Arrange
    int current_socket;
    int mock_socket_rtn = 4;
    mock_set_return_value(&socket_mock, &mock_socket_rtn);

    // Act
    create_socket(&current_socket);

    // Assert
    ck_assert_int_eq(current_socket, mock_socket_rtn);
    ck_assert_int_eq(mock_get_call_count(&socket_mock), 1);
    ck_assert_int_eq(mock_get_call_count(&perror_mock), 0);
}
END_TEST

START_TEST(it_throws_perror_on_socket_create_error)
{
    // Arrange
    int current_socket;
    int mock_socket_rtn = -1;
    mock_set_return_value(&socket_mock, &mock_socket_rtn);

    // Act
    create_socket(&current_socket);

    // Assert
    ck_assert_int_eq(mock_get_call_count(&perror_mock), 1);
}
END_TEST

Suite *make_create_socket_test_suite(void)
{
    Suite *s;
    TCase *tc;

    s = suite_create("Create Socket");
    tc = tcase_create("Core");

    tcase_add_checked_fixture(tc, &setup, &teardown);

    tcase_add_test(tc, it_creates_a_socket);
    tcase_add_test(tc, it_throws_perror_on_socket_create_error);
    suite_add_tcase(s, tc);

    return s;
}

