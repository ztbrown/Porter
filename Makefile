export BUILDDIR=$(CURDIR)
export CFLAGS+= -std=c99

all:
	$(MAKE) -C src

check:
	docker run -it --rm -P -v $(CURDIR):/code -w="/code" lib-check /bin/sh -c "export CK_FORK=no && make -C tests && ./tests/test_suite"
