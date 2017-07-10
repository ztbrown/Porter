TARGET = test_suite
SRCS = ../src/accept_connections.c ../src/http_utils.c ../src/daemonize.c ../src/create_socket.c ../src/bind_socket.c
TESTS = ./*_test.c

all:
	gcc $(CFLAGS) $(SRCS) $(TESTS) test_suite.c -o $(TARGET) ../lib/libmock.a `pkg-config --cflags --libs check` `cat $(TESTS) | ../bin/generate_mock_linker_flags.sh`
