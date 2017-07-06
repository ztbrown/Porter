export BUILDDIR=$(CURDIR)
export CFLAGS+= -std=c99

all:
	$(MAKE) -C src

check:
	$(MAKE) -C tests
