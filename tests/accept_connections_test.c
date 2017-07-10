#include <check.h>

#include "../lib/mock.h"
#include "../src/accept_connections.h"

SIMULACRUM(int, accept, 3, int, struct sockaddr *, socklen_t *)
SIMULACRUM(void, handle, 1, int)
SIMULACRUM(void, close, 1, int)

static void setup()
{
}

static void teardown()
{
    mock_reset_call_count(&accept_mock);
    mock_reset_call_count(&handle_mock);
    mock_reset_call_count(&close_mock);
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

Suite *make_accept_connections_test_suite()
{
    Suite *s;
    TCase *tc;

    s = suite_create("Accept Connections");
    tc = tcase_create("Core");

    tcase_add_checked_fixture(tc, &setup, &teardown);

    tcase_add_test(tc, it_calls_accept_and_sets_the_connecting_socket);

    suite_add_tcase(s, tc);

    return s;
}
