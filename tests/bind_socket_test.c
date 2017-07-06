#include <check.h>
#include <stdlib.h>
#include <stdio.h>

#include "../lib/mock.h"
#include "../src/bind_socket.h"
#include "bind_socket_test.h"

static void setup()
{

}

static void teardown()
{

}

START_TEST(it_sets_socket_internet_family_address_and_port)
{
    // Arrange 
    struct sockaddr_in address = {0};

    // convert port to network short (Big Endian)
    int expected_port = htons(80);

    // Act
    bind_socket(&address, 0, 80);

    // Assert
    
    ck_assert_int_eq(address.sin_family, AF_INET);
    ck_assert_int_eq(address.sin_addr.s_addr, INADDR_ANY);
    ck_assert_int_eq(address.sin_port, expected_port);
}
END_TEST

Suite *make_bind_socket_test_suite(void)
{
    Suite *s;
    TCase *tc;

    s = suite_create("Bind Socket");
    tc = tcase_create("Core");

    tcase_add_checked_fixture(tc, &setup, &teardown);

    tcase_add_test(tc, it_sets_socket_internet_family_address_and_port);
    suite_add_tcase(s, tc);

    return s;
}

