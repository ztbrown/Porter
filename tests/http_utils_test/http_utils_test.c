#include <check.h>
#include <stdlib.h>
#include <stdio.h>

#include "http_utils_test.h"

#include "../../src/http_utils.h"
#include "../../lib/mock.h"

SIMULACRUM(int, send, 4, int, char*, int, int)
//IMULACRUM(void, fopen, 2, char*, char*)
//IMULACRUM(int, fgets, 3, char*, int, FILE*)

struct send_args_s {
    char *message;
    int socket;
    int length;
    int flags;
};
struct send_args_s send_args;

void send_callback(int socket, char *message, int length, int flags)
{
    send_args.message = malloc(length + 1);
    memcpy(send_args.message, message, length + 1);

    send_args.socket = socket;
    send_args.length = length;
    send_args.flags = flags;
}

static void setup()
{
    int success = 0;
    mock_set_return_value(&send_mock, &success);
}

static void teardown()
{
    mock_reset_call_count(&send_mock);
}

/*char *fgets_returns[] = {"text/html                  html htm"}; 
int fgets_callback(char* line, int length, FILE *file)
{
    int index = mock_get_call_count(&fgets_mock);
    if (fgets_returns[index]) {
        strcpy(line, fgets_returns[index]);
    }
    else {
        // this is crazy janky
        mock_set_return_value(&fgets_mock, NULL);
    }
}
*/

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

// send_string

START_TEST(it_sends_string_over_connecting_socket)
{
    // Arrange
    char *message = "Hello World!";
    int connecting_socket = 0;

    mock_set_callback(&send_mock, &send_callback);

    // Act
    send_string(message, connecting_socket);

    // Assert
    ck_assert_int_eq(mock_get_call_count(&send_mock), 1);
    ck_assert_str_eq(send_args.message, message);
    ck_assert_int_eq(send_args.socket, connecting_socket);

    // Clean up
    free(send_args.message);
}
END_TEST

// send_header

START_TEST(it_properly_formats_the_200_http_response_header)
{
    // Arrange
    char *response = "200 OK";
    char *mime = "text/html";
    char *expected = "\r\nHTTP/1.1 200 OK";
                     "\r\nContent-Type: text/html"
                     "\r\nServer: PT06"
                     "\r\nContent-Length: 1337"
                     "\r\nDate: ";
    int header_len = strlen(expected);

    // Act
    send_header(response, mime, 1337, 0);

    // Assert
    ck_assert_int_eq(mock_get_call_count(&send_mock), 1);
    send_args.message[header_len] = '\0';
    ck_assert_str_eq(send_args.message, expected);
}
END_TEST

START_TEST(it_properly_formats_the_400_http_response_header)
{
    // Arrange
    char *response = "400 Bad Request";
    char *mime = "text/html";
    char *expected = "\r\nHTTP/1.1 400 Bad Request";
                     "\r\nContent-Type: text/html"
                     "\r\nServer: PT06"
                     "\r\nContent-Length: 1337"
                     "\r\nDate: ";
    int header_len = strlen(expected);

    // Act
    send_header(response, mime, 1337, 0);

    // Assert
    ck_assert_int_eq(mock_get_call_count(&send_mock), 1);
    send_args.message[header_len] = '\0';
    ck_assert_str_eq(send_args.message, expected);
}
END_TEST

// scan 

START_TEST(it_scans_text_gets_next_word_and_word_length)
{
    // Arrange
    char *input = "GET /index.html HTTP/1.1";
    char output[200];
    int filename_length;

    // Act
    filename_length = scan(input, output, 5, 200);

    // Assert
    ck_assert_int_eq(filename_length, 11);
    ck_assert_str_eq(output, "index.html");
}
END_TEST

// check_mime

START_TEST(it_checks_mime_type)
{
    // Arrange
    char *extension = "html";
    char mime[200] = {0};

    mime_file = malloc(600);
    // TODO: mock system call to fopen
    strcpy(mime_file, "mime.types");

    // Act
    check_mime(extension, mime);

    // Assert
    ck_assert_str_eq(mime, "text/html");

    // Clean up 
    free(mime_file);
}
END_TEST

Suite *make_http_utils_test_suite()
{
    Suite *s;
    TCase *tc, *tc2;

    s = suite_create("Http Utils");
    tc = tcase_create("getRequestType");

    tcase_add_checked_fixture(tc, &setup, &teardown);

    tcase_add_test(tc, it_gets_request_type_for_GET);
    tcase_add_test(tc, it_gets_request_type_for_HEAD);
    
    tcase_add_test(tc, it_gets_request_type_for_POST);
    tcase_add_test(tc, it_returns_error_code_for_invalid_request);

    suite_add_tcase(s, tc);

    tc = tcase_create("send_string");
    tcase_add_checked_fixture(tc, &setup, &teardown);

    tcase_add_test(tc, it_sends_string_over_connecting_socket);

    suite_add_tcase(s, tc);

    tc = tcase_create("send_header");
    tcase_add_checked_fixture(tc, &setup, &teardown);

    tcase_add_test(tc, it_properly_formats_the_200_http_response_header);
    tcase_add_test(tc, it_properly_formats_the_400_http_response_header);

    suite_add_tcase(s, tc);

    tc = tcase_create("scan");
    tcase_add_checked_fixture(tc, &setup, &teardown);

    tcase_add_test(tc, it_scans_text_gets_next_word_and_word_length);

    suite_add_tcase(s, tc);

    tc = tcase_create("check_mime");
    tcase_add_checked_fixture(tc, &setup, &teardown);

    tcase_add_test(tc, it_checks_mime_type);

    suite_add_tcase(s, tc);


    return s;
}
