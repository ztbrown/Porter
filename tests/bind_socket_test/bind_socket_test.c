#include <check.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "../../lib/mock.h"
#include "../../src/bind_socket.h"
#include "bind_socket_test.h"

SIMULACRUM(void, exit, 1, int)
SIMULACRUM(int, bind, 3, int, struct sockaddr *, size_t)
struct bind_args_s
{
    int socket;
    struct sockaddr_in *address;
    size_t address_size;
};

struct bind_args_s captured_args;

static void bind_callback(int socket, struct sockaddr_in *address, size_t size)
{
    captured_args.socket = socket;
    captured_args.address = address;
    captured_args.address_size = size;
}

static void setup()
{
}

static void teardown()
{
    mock_reset_call_count(&bind_mock);
}

START_TEST(it_binds_the_socket)
{
    // Arrange
    struct sockaddr_in address = {0};
    int expected_socket = 4;
    struct addrinfo addr_info = {
        .ai_addr = (struct sockaddr *)&address,
        .ai_addrlen = sizeof(address)
    };
    int success = 0;

    mock_set_return_value(&bind_mock, &success);
    mock_set_callback(&bind_mock, &bind_callback);

    // Act
    bind_socket(expected_socket, &addr_info);

    // Assert
    ck_assert_int_eq(mock_get_call_count(&bind_mock), 1);
    ck_assert_int_eq(captured_args.socket, expected_socket);
    ck_assert_ptr_eq(captured_args.address, &address);
    ck_assert_int_eq(captured_args.address_size, sizeof(address));
}
END_TEST

Suite *make_bind_socket_test_suite(void)
{
    Suite *s;
    TCase *tc;

    s = suite_create("Bind Socket");
    tc = tcase_create("Core");

    tcase_add_checked_fixture(tc, &setup, &teardown);

    tcase_add_test(tc, it_binds_the_socket);
    suite_add_tcase(s, tc);

    return s;
}

