TARGET = test_suite
TMP = $(wildcard ../../src/*.c)
SRCS = $(filter-out ../../src/GoHttp.c, $(TMP))
TESTS = $(wildcard ./*_test.c)

SRC_OBJS = $(SRCS:.c=.o)
TEST_OBJS = $(TESTS:.c=.o)

all: $(SRC_OBJS) $(TEST_OBJS)
	gcc $(CFLAGS) $^ test_suite.c -o $(TARGET) ../../lib/libmock.a `pkg-config --cflags --libs check` `cat $(TESTS) | ../../bin/generate_mock_linker_flags.sh` && ./test_suite

../src/%.o: ../src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
