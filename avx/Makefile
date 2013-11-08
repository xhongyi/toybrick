popcount.o: popcount.c popcount.h
	gcc -O2 -march=native -c popcount.c -o popcount.o
	
popcount: popcount.h popcount.o popcountMain.c
	gcc -O2 -march=native popcount.o popcountMain.c -o popcount

bit_convert.o: bit_convert.c bit_convert.h
	gcc -O2 -march=native -c bit_convert.c -o bit_convert.o
	
bit_convert: bit_convert.o bit_convertMain.c
	gcc -O2 -march=native bit_convert.o bit_convertMain.c -o bit_convert

string_cp: string_cp.c
	g++ -O2 -DALIGN_DATA -march=native string_cp.c -o string_cp