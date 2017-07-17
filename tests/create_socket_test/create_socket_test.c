#include <check.h>
#include <stdlib.h>
#include <stdio.h>

#include <netdb.h>

#include "../../lib/mock.h"
#include "../../src/create_socket.h"
#include "create_socket_test.h"

SIMULACRUM(int, socket, 3, int, int, int)
SIMULACRUM(void, perror, 1, char*)
SIMULACRUM(void, exit, 1, int)

static struct addrinfo socket_argument_captor;
static void socket_callback(int family, int socktype, int protocol)
{
    socket_argument_captor.ai_family = family;
    socket_argument_captor.ai_socktype = socktype;
    socket_argument_captor.ai_protocol = protocol;
}

static void setup()
{
    int defrtn = 0;
    memset(&socket_argument_captor, 0, sizeof(socket_argument_captor));
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
    struct addrinfo address_info = {
        .ai_flags = 0,
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = 0
    };

    mock_set_return_value(&socket_mock, &mock_socket_rtn);
    mock_set_callback(&socket_mock, &socket_callback);

    // Act
    create_socket(&current_socket, &address_info);

    // Assert
    ck_assert_int_eq(current_socket, mock_socket_rtn);
    ck_assert_int_eq(socket_argument_captor.ai_family, address_info.ai_family);
    ck_assert_int_eq(socket_argument_captor.ai_socktype, address_info.ai_socktype);
    ck_assert_int_eq(socket_argument_captor.ai_protocol, address_info.ai_protocol);
    ck_assert_int_eq(mock_get_call_count(&socket_mock), 1);
    ck_assert_int_eq(mock_get_call_count(&perror_mock), 0);
}
END_TEST

START_TEST(it_throws_perror_on_socket_create_error)
{
    // Arrange
    int current_socket;
    int mock_socket_rtn = -1;
    struct addrinfo address_info = { 0 };

    mock_set_return_value(&socket_mock, &mock_socket_rtn);

    // Act
    create_socket(&current_socket, &address_info);

    // Assert
    ck_assert_int_eq(mock_get_call_count(&perror_mock), 1);
}
END_TEST


SIMULACRUM(void, getaddrinfo, 4, const char *, const char *, const struct addrinfo *, struct addrinfo **)
static void getaddrinfo_callback(const char *node,
                                 const char *service,
                                 const struct addrinfo *hints,
                                 struct addrinfo **res)
{
    (*res) = malloc(sizeof(struct addrinfo));
    (*res)->ai_family = AF_INET;
    (*res)->ai_socktype = SOCK_STREAM;
    (*res)->ai_protocol = IPPROTO_TCP;
}

START_TEST(it_gets_network_information)
{
    // Arrange
    struct addrinfo *res;
    mock_set_callback(&getaddrinfo_mock, &getaddrinfo_callback);

    // Act
    get_network_info(&res, 80);

    // Assert
    ck_assert_int_eq(mock_get_call_count(&getaddrinfo_mock), 1);
    ck_assert_int_eq(res->ai_family, AF_INET);
    ck_assert_int_eq(res->ai_socktype, SOCK_STREAM);
    ck_assert_int_eq(res->ai_protocol, IPPROTO_TCP);

    // Tear down
    free(res);
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
    tcase_add_test(tc, it_gets_network_information);
    suite_add_tcase(s, tc);

    return s;
}

