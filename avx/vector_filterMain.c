/*
 * vector_filterMain.c
 *
 *  Created on: Nov 12, 2013
 *      Author: hxin
 */

#ifndef BOOST_PP_IS_ITERATING

//#include "print.h"
#include <stdint.h>
#include "vector_filter.h"
#include <stdio.h>
#include <string.h>
#include <nmmintrin.h>
#include <emmintrin.h>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/iteration.hpp>

#define SSE_BIT_LENGTH		128
#define BASE_SIZE			2
#define SSE_BASE_NUM		BOOST_PP_DIV(SSE_BIT_LENGTH, BASE_SIZE)
#define BYTE_BASE_NUM		BOOST_PP_DIV(8, BASE_SIZE)
#define SSE_BYTE_NUM		BOOST_PP_DIV(SSE_BIT_LENGTH, 8)

MASK_SSE_END[SSE_BIT_LENGTH * SSE_BYTE_NUM] = {

#define BOOST_PP_ITERATION_LIMITS	(0, SSE_BIT_LENGTH - 1)
#define BOOST_PP_FILENAME_1			"vector_filterMain.c" // this file
#include BOOST_PP_ITERATE()

#else // BOOST_PP_IS_ITERATING

#define I BOOST_PP_ITERATION()
#define PRINT_DATA(z, n, data) data

		BOOST_PP_ENUM(SSE_BYTE_NUM, PRINT_DATA, I)

#endif
#ifndef BOOST_PP_IS_ITERATING
};

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

	for (int i = 0; i < SSE_BIT_LENGTH * SSE_BYTE_NUM; i++) {
		if (i % SSE_BYTE_NUM == 0)
			printf("\n");
		printf("%x ", MASK_SSE_END[i]);
	}

	printf("\n");

//	while (repeat_count--)
//		bit_vec_filter_sse_simulate(read_t, ref_t, length, error, average_loc);
	if (bit_vec_filter_sse(read_t, ref_t, length, error))
		printf("Pass Filter\n");
	else
		printf("Fail Filter\n");

	return 0;

}

#endif
