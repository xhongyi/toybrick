EXECUTABLE = countPassFilterHamming countPassFilter bit_convert vector_filter string_cp #ssse3_popcount test_modifier

CXX = g++-4.7

CC = g++-4.7

LD = ld

LDFLAGS = -r

CFLAGS = -O3 -msse4.2 -I .
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
endif
ifeq ($(UNAME_S),Darwin)
	CFLAGS += -I/opt/local/include
endif

#CFLAGS = -O3 -march=native -P -E

all: $(EXECUTABLE)

print.o: print.c print.h
	$(CC) $(CFLAGS) -c $< -o $@

SRS.o: SRS.c SRS.h
	$(CC) $(CFLAGS) -c $< -o $@
	
bit_convert.o: bit_convert.c bit_convert.h
	$(CC) $(CFLAGS) -c $< -o $@
	
bit_convert: print.o bit_convert.o bit_convertMain.c
	$(CC) $(CFLAGS) $^ -o $@

vector_filter.o: vector_filter.c vector_filter.h
	$(CC) $(CFLAGS) -c $< -o $@

mask.o: mask.c mask.h
	$(CC) $(CFLAGS) -c $< -o $@

sse.o: mask.o print.o bit_convert.o SRS.o vector_filter.o
	$(LD) $(LDFLAGS) $^ -o $@

vector_filter: mask.o print.o bit_convert.o SRS.o vector_filter.o vector_filterMain.c
	$(CC) $(CFLAGS) $^ -o $@

countPassFilter: mask.o print.o bit_convert.o SRS.o vector_filter.o countPassFilter.cc
	$(CC) $(CFLAGS) $^ -o $@

countPassFilterHamming: mask.o print.o bit_convert.o SRS.o vector_filter.o countPassFilterHamming.cc
	$(CC) $(CFLAGS) $^ -o $@

timeSSE: timeSSE.c
	$(CC) $(CFLAGS) $< -o $@

string_cp: print.o string_cp.c
	$(CXX) $(CFLAGS) $^ -o $@
		
read_modifier.o: read_modifier.c read_modifier.h
	$(CXX) $(CFLAGS) -c $< -o $@
	
test_modifier: mask.o print.o bit_convert.o SRS.o vector_filter.o read_modifier.o test_modifier.c 
	$(CXX) $(CFLAGS) $^ -o $@
		
#ssse3_popcount: ssse3_popcount.c
#	$(CXX) $(CFLAGS) $< -o $@
	

	
.PHONY : clean

clean:
	rm *.o $(EXECUTABLE)
