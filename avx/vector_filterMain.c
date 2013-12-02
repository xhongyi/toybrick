/*
 * vector_filterMain.c
 *
 *  Created on: Nov 12, 2013
 *      Author: hxin
 */

//#include "print.h"
#include <stdint.h>
#include "vector_filter.h"
#include <stdio.h>
#include <string.h>
#include <nmmintrin.h>
#include <emmintrin.h>

#define _MAX_LENGTH_ 320

char read_t[_MAX_LENGTH_] __aligned__;
char ref_t[_MAX_LENGTH_] __aligned__;

//uint8_t read_bit_t[_MAX_LENGTH_ / 4] __aligned__;
//uint8_t ref_bit_t[_MAX_LENGTH_ / 4] __aligned__;

int main(int argc, char* argv[]) {

	int length = 128;
	int error = 0;
	int repeat_count = 10000;
	int average_loc = 10;

	strcpy(read_t,
			"TCGCTAGTAGCCGGAATAACAGGTAGGCCTACATTTTCTATACGGCGCCGGCAACCTTGAGGGGCCGCGCCCCGTTACACTTTATACGTTTCCCTTGCAAGCCTTCGTGTCGGAGCATATGTATATGG");
	strcpy(ref_t,
			"TCGCTAGTACCGGAATAACAGGTAGGCCTGCATTTTCTATACGGCGCCGGCAACCTTGAGGGGCCGCGCCCCGTTACACTTTATACGTTTCCCTTGCAAGCCTTCGTGTCGGAGCATATGTATATGGA");

	if (argc >= 2)
		length = atoi(argv[1]);
	if (argc >= 3)
		error = atoi(argv[2]);
	if (argc >= 4)
		repeat_count = atoi(argv[3]);
	if (argc >= 5)
		average_loc = atoi(argv[4]);

//	while (repeat_count--)
//		bit_vec_filter_sse_simulate(read_t, ref_t, length, error, average_loc);
	if (bit_vec_filter_sse(read_t, ref_t, length, error))
		printf("Pass Filter\n");
	else
		printf("Fail Filter\n");

	return 0;

}
