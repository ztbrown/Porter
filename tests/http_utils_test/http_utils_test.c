#include <check.h>

#include "http_utils_test.h"

#include "../../src/http_utils.h"
#include "../../lib/mock.h"

static void setup()
{
}

static void teardown()
{
}

// getRequestType

START_TEST(it_gets_request_type_for_GET)
{
    // Arrange
    char *request_type = "GET";
    int actual = -1;

    // Act
    actual = getRequestType(request_type);

    // Assert
    ck_assert_int_eq(actual, 1);
}
END_TEST

START_TEST(it_gets_request_type_for_HEAD)
{
    // Arrange
    char *request_type = "HEAD";
    int actual = -1;

    // Act
    actual = getRequestType(request_type);

    // Assert
    ck_assert_int_eq(actual, 2);
}
END_TEST

START_TEST(it_gets_request_type_for_POST)
{
    // Arrange
    char *request_type = "POST";
    int actual = -1;

    // Act
    actual = getRequestType(request_type);

    // Assert
    ck_assert_int_eq(actual, 0);
}
END_TEST

START_TEST(it_returns_error_code_for_invalid_request)
{
    // Arrange
    char *request_type = "NOTHING";
    int actual = 0;

    // Act
    actual = getRequestType(request_type);

    // Assert
    ck_assert_int_eq(actual, -1);
}
END_TEST

Suite *make_http_utils_test_suite()
{
    Suite *s;
    TCase *tc;

    s = suite_create("Http Utils");
    tc = tcase_create("Core");

    tcase_add_checked_fixture(tc, &setup, &teardown);

    tcase_add_test(tc, it_gets_request_type_for_GET);
    tcase_add_test(tc, it_gets_request_type_for_HEAD);
    tcase_add_test(tc, it_gets_request_type_for_POST);
    tcase_add_test(tc, it_returns_error_code_for_invalid_request);

    suite_add_tcase(s, tc);

    return s;
}
