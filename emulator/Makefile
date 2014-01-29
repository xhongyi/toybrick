EXECUTABLE = emulate_vector

CXX = g++

CC = g++

CFLAGS = -g -march=native -I .

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
endif
ifeq ($(UNAME_S),Darwin)
	CFLAGS += -I/opt/local/include
endif

#CFLAGS = -O3 -march=native -P -E

all: $(EXECUTABLE)

emulate_vector.o: emulate_vector.cc emulate_vector.h
	$(CC) $(CFLAGS) -c $< -o $@

emulate_vector: emulate_vector.cc emulate_vector_main.cc
	$(CC) $(CFLAGS) $^ -o $@

.PHONY : clean

clean:
	rm *.o $(EXECUTABLE)
