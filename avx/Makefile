EXECUTABLE = popcount bit_convert vector_filter string_cp ssse3_popcount shift

CXX = g++

CC = gcc

CFLAGS = -O2 -march=native

all: $(EXECUTABLE)

print.o: print.c print.h
	$(CC) $(CFLAGS) -c $< -o $@

popcount.o: popcount.c popcount.h
	$(CC) $(CFLAGS) -c $< -o $@
	
popcount: popcount.o print.o popcountMain.c
	$(CC) $(CFLAGS) $^ -o $@

bit_convert.o: bit_convert.c bit_convert.h
	$(CC) $(CFLAGS) -c $< -o $@
	
bit_convert: print.o bit_convert.o bit_convertMain.c
	$(CC) $(CFLAGS) $^ -o $@

vector_filter.o: vector_filter.c vector_filter.h
	$(CC) $(CFLAGS) -c $< -o $@

vector_filter: print.o bit_convert.o popcount.o vector_filter.o vector_filterMain.c
	$(CC) $(CFLAGS) $^ -o $@

shift: print.o vector_filter.o popcount.o bit_convert.o shiftMain.c
	$(CC) $(CFLAGS) $^ -o $@

timeSSE: timeSSE.c
	$(CC) $(CFLAGS) $< -o $@

string_cp: print.o string_cp.c
	$(CXX) $(CFLAGS) $^ -o $@
	
ssse3_popcount: ssse3_popcount.c
	$(CXX) $(CFLAGS) $< -o $@
	

	
.PHONY : clean

clean:
	rm *.o $(EXECUTABLE)