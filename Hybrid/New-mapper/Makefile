SHELL := /bin/bash

CC = g++
CFLAGS = -c -O3 -msse4.2 -I .
LDFLAGS = -lz -lm

all: main

main: RefGenome.o main.cc
	$(CC) RefGenome.o bit_convert.o print.o main.cc -o main $(LDFLAGS)

RefGenome.o: RefGenome.cc bit_convert.o
	$(CC) RefGenome.cc $(CFLAGS)

bit_convert.o: bit_convert.c print.o
	$(CC) bit_convert.c $(CFLAGS)

print.o: print.c
	$(CC) print.c $(CFLAGS) 

clean:
	rm -f *.o main