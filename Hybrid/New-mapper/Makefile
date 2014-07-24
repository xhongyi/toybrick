SHELL := /bin/bash

CC = g++
CFLAGS = -c -O3 -msse -msse2 -msse4.2
LDFLAGS = -lz -lm -msse -msse2 -msse4.2

all: main

main: RefGenome.o main.cc
	$(CC) RefGenome.o main.cc -o main $(LDFLAGS)

RefGenome.o: RefGenome.cc
	$(CC) RefGenome.cc $(CFLAGS)

clean:
	rm -f *.o main