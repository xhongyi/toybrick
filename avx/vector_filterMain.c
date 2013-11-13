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
#include <nmmintrin.h>
#include <emmintrin.h>

uint8_t buff[32] __aligned__;

int main(int argc, char* argv[]) {
	int shift_count = 0;

	if (argc == 2)
		shift_count = atoi(argv[1]);

	int i;
	for (i = 0; i < 16; i++)
		buff[i] = 0x0;
	for (i = 16; i < 32; i++)
		buff[i] = 0xff;
//	buff[15] = 0xc0;

	printf("Data: ");
	printbytevector(buff, 32);
	printf("\n");

	__m128i *prev, *curr;

	prev = (__m128i *) buff;
	curr = (__m128i *) (buff + 16);

	__m128i zerofilled;
	zerofilled = _mm_set1_epi8(0x0);

//	*curr = shift_right_sse(*prev, *curr, 1);
	*prev = shift_left_sse(*prev, *curr, 1);

//	*curr = _mm_slli_epi16(*curr, 2);

	printf("Data: ");
	printbytevector(buff, 32);
	printf("\n");
	return 0;

}
