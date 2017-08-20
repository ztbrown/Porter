TEST_SUITES = $(wildcard ./*_test)

all: $(TEST_SUITES)
	@for a in $^; do \
		make -C $$a; \
	done;

clean: $(TEST_SUITES)
	@for a in $^; do 			\
		cd $$a;		   			\
		rm -rf *.o test_suite;  \
		cd -;		 			\
	done;

