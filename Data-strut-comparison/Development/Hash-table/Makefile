CPPFLAGS=-O3

all: ht_benchmark
#	rm *.o

SeedTree.o: SeedTree.cc SeedTree.h
	g++ $(CPPFLAGS) -c $<

HashTable.o: HashTable.cc HashTable.h
	g++ $(CPPFLAGS) -c $<

RefDB.o: RefDB.cc RefDB.h
	g++ $(CPPFLAGS) -c $<

KmerHash.o: KmerHash.cc KmerHash.h
	g++ $(CPPFLAGS) -c $<

LongKmerHash.o: LongKmerHash.cc LongKmerHash.h
	g++ $(CPPFLAGS) -c $<

LongHashTable.o: LongHashTable.cc LongHashTable.h
	g++ $(CPPFLAGS) -c $<

ht_benchmark: ht_benchmark.cc SeedTree.o HashTable.o RefDB.o KmerHash.o LongKmerHash.o LongHashTable.o
	g++ $(CPPFLAGS) -o $@ $^

.PHONY: clean copy

clean:
	rm *.o read_hotspots
