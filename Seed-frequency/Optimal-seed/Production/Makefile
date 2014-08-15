all: brute_force_optimization.so

brute_force_optimization.so: brute_force_optimization.c
	gcc -shared -pthread -fPIC -fwrapv -O2 -Wall -fno-strict-aliasing -I/usr/include/python2.7 -o $@ $<

brute_force_optimization.c: brute_force_optimization.py
	cython -a $<