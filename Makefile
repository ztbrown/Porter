export BUILDDIR=$(CURDIR)
export CFLAGS+= -Wall -Werror -std=c99

all: clean
	$(MAKE) -C src

check: clean
	docker run -it --rm -P -v $(CURDIR):/code -w="/code" lib-check /bin/sh -c "export CK_FORK=no && make -s -C tests"

run-suite: clean
	docker run -it --rm -P -v $(CURDIR):/code -w="/code/tests" lib-check /bin/sh -c "export CK_FORK=no && make -C $(SUITE)"

clean:
	rm -rf **/*.o && $(MAKE) clean -C tests
