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

uint8_t read_bit_t[_MAX_LENGTH_ / 4] __aligned__;
uint8_t ref_bit_t[_MAX_LENGTH_ / 4] __aligned__;

int main(int argc, char* argv[]) {
//	int shift_count = 0;
//
//	if (argc == 2)
//		shift_count = atoi(argv[1]);
//
//	int i;
//	for (i = 0; i < 16; i++)
//		buff[i] = 0x0;
//	for (i = 16; i < 32; i++)
//		buff[i] = 0xff;
////	buff[15] = 0xc0;
//
//	printf("Data: ");
//	printbytevector(buff, 32);
//	printf("\n");
//
//	__m128i *prev, *curr;
//
//	prev = (__m128i *) buff;
//	curr = (__m128i *) (buff + 16);
//
//	__m128i zerofilled;
//	zerofilled = _mm_set1_epi8(0x0);
//
////	*curr = shift_right_sse(*prev, *curr, 1);
//	*prev = shift_left_sse(*prev, *curr, 1);
//
////	*curr = _mm_slli_epi16(*curr, 2);
//
//	printf("Data: ");
//	printbytevector(buff, 32);
//	printf("\n");

	int length = 128;
	int error = 0;
	int repeat_count = 10000;
	int average_loc = 10;

	strcpy(read_t,
			"ACGCTAGTAGCCGGAATAACAGGTAGGCCTACATTTTCTATACGGCGCCGGCAACCTTGAGGGGCCGCGCCCCGTTACACTTTATACGTTTCCCTTGCAAGCCTTCGTGTCGGAGCATATGTATATGG");
	strcpy(ref_t,
			"ACGCTAGTAGCCGGAATAACAGGTAGGCCTACATTTTCTATACGGCGCCGGCAACCTTGAGGGGCCGCGCCCCGTTACACTTTATACGTTTCCCTTGCAAGCCTTCGTGTCGGAGCATATGTATATGG");

	if (argc >= 2)
		length = atoi(argv[1]);
	if (argc >= 3)
		error = atoi(argv[2]);
	if (argc >= 4)
		repeat_count = atoi(argv[3]);
	if (argc >= 5)
		average_loc = atoi(argv[4]);

	while (repeat_count--)
		bit_vec_filter_sse_simulate(read_t, ref_t, length, error, average_loc);
//	if (bit_vec_filter_sse(read_t, ref_t, length, error))
//		printf("Pass Filter\n");
//	else
//		printf("Fail Filter\n");

	return 0;

}
