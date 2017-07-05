all: 
	gcc ../src/daemonize.c ../src/create_socket.c create_socket_test.c daemonize_test.c test_suite.c -o test_suite ../lib/libmock.a `pkg-config --cflags --libs check` `cat daemonize_test.c create_socket_test.c | ../bin/generate_mock_linker_flags.sh`
