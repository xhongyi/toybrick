/*
 * vector_ed.c
 *
 *  Created on: Nov 8, 2013
 *      Author: hxin
 */

#include "vector_filter.h"
#include <nmmintrin.h>
#include <tmmintrin.h>
#include <stdio.h>

__m128i alignr_helper(__m128i a, __m128i b, int shift_num) {
	switch (shift_num) {
	case 1:
		return _mm_alignr_epi8(a, b, 1);
		break;
	case 2:
		return _mm_alignr_epi8(a, b, 2);
		break;
	case 3:
		return _mm_alignr_epi8(a, b, 3);
		break;
	case 4:
		return _mm_alignr_epi8(a, b, 4);
		break;
	case 5:
		return _mm_alignr_epi8(a, b, 5);
		break;
	case 6:
		return _mm_alignr_epi8(a, b, 6);
		break;
	case 7:
		return _mm_alignr_epi8(a, b, 7);
		break;
	case 8:
		return _mm_alignr_epi8(a, b, 8);
		break;
	case 9:
		return _mm_alignr_epi8(a, b, 9);
		break;
	case 10:
		return _mm_alignr_epi8(a, b, 10);
		break;
	case 11:
		return _mm_alignr_epi8(a, b, 11);
		break;
	case 12:
		return _mm_alignr_epi8(a, b, 12);
		break;
	case 13:
		return _mm_alignr_epi8(a, b, 13);
		break;
	case 14:
		return _mm_alignr_epi8(a, b, 14);
		break;
	case 15:
		return _mm_alignr_epi8(a, b, 15);
		break;
	default:
		printf("error! shift_num: %d\n", shift_num);
		exit(1);
		break;
	}
}

__m128i shift_right_sse(__m128i pri_vec, __m128i vec, int shift_num) {
	if (shift_num % 4 == 0)
		return alignr_helper(pri_vec, vec, 16 - shift_num / 4);

	__m128i carryover;
	__m128i shiftee;
	__m128i mask;

	carryover = alignr_helper(pri_vec, vec, 15 - shift_num / 4);
	carryover = _mm_slli_epi64(carryover, (4 - shift_num % 4) * 2);

	if (shift_num > 4)
		shiftee = alignr_helper(pri_vec, vec, 15 - shift_num / 4);
	else
		shiftee = vec;

	shiftee = _mm_srli_epi64(shiftee, (shift_num % 4) * 2);

	return _mm_or_si128(shiftee, carryover);
}
