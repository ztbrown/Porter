export BUILDDIR=$(CURDIR)
export CFLAGS+= -Wall -Werror -std=c99

all: clean
	$(MAKE) -C src

check: clean
	docker run -it --rm -P -v $(CURDIR):/code -w="/code" lib-check /bin/sh -c "export CK_FORK=no && make -C tests && ./tests/test_suite"

leak-check: clean
	docker run -it --rm -P -v $(CURDIR):/code -w="/code" lib-check /bin/sh -c "export CK_FORK=no && make -C tests && valgrind --leak-check=full ./tests/test_suite"

clean:
	rm -rf **/*.o tests/test_suite
