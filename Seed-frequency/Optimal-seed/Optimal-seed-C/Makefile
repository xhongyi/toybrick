TARGETS = fast_brute
PYTHON_BINARY_PATH = /usr/include/python2.7

COMPILER = gcc
CFLAGS = -std=c99

all: compiled_targets

compiled_targets:
	for target in $(TARGETS); do \
		swig -python $$target.i; \
		$(COMPILER) $(CFLAGS) -c -fPIC $$target.c $(addsuffix '_wrap.c', $$target) -I$(PYTHON_BINARY_PATH); \
		$(COMPILER) $(CFLAGS) -shared $$target.o $(addsuffix '_wrap.o', $$target) -o $(addprefix '_', $$target.so); \
	done

clean:
	rm -rf *.o *.pyc *.so
	for target in $(TARGETS); do \
		rm -rf $(addsuffix '_wrap.c', $$target); \
		rm -rf $$target.py; \
	done