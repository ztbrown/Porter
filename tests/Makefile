TEST_SUITES = $(wildcard ./*_test)

all: $(TEST_SUITES)
	@for a in $^; do \
		make -C $$a; \
	done;

