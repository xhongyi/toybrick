/*
 * vector_ed.c
 *
 *  Created on: Nov 8, 2013
 *      Author: hxin
 */

#include "print.h"
#include "vector_filter.h"
#include <nmmintrin.h>
#include <tmmintrin.h>
#include <stdio.h>

/*
 * By little endians, left shift should actually be right shift in x86 convention
 */

__m128i right_alignr_helper(__m128i prev, __m128i curr, int shift_num) {
	switch (shift_num) {
	case 0:
		return _mm_alignr_epi8(curr, prev, 16);
		break;
	case 1:
		return _mm_alignr_epi8(curr, prev, 15);
		break;
	case 2:
		return _mm_alignr_epi8(curr, prev, 14);
		break;
	case 3:
		return _mm_alignr_epi8(curr, prev, 13);
		break;
	case 4:
		return _mm_alignr_epi8(curr, prev, 12);
		break;
	case 5:
		return _mm_alignr_epi8(curr, prev, 11);
		break;
	case 6:
		return _mm_alignr_epi8(curr, prev, 10);
		break;
	case 7:
		return _mm_alignr_epi8(curr, prev, 9);
		break;
	case 8:
		return _mm_alignr_epi8(curr, prev, 8);
		break;
	case 9:
		return _mm_alignr_epi8(curr, prev, 7);
		break;
	case 10:
		return _mm_alignr_epi8(curr, prev, 6);
		break;
	case 11:
		return _mm_alignr_epi8(curr, prev, 5);
		break;
	case 12:
		return _mm_alignr_epi8(curr, prev, 4);
		break;
	case 13:
		return _mm_alignr_epi8(curr, prev, 3);
		break;
	case 14:
		return _mm_alignr_epi8(curr, prev, 2);
		break;
	case 15:
		return _mm_alignr_epi8(curr, prev, 1);
		break;
	default:
		printf("error! shift_num: %d\n", shift_num);
		exit(1);
		break;
	}
}

__m128i left_alignr_helper(__m128i curr, __m128i next, int shift_num) {
	switch (shift_num) {
	case 0:
		return _mm_alignr_epi8(next, curr, 0);
		break;
	case 1:
		return _mm_alignr_epi8(next, curr, 1);
		break;
	case 2:
		return _mm_alignr_epi8(next, curr, 2);
		break;
	case 3:
		return _mm_alignr_epi8(next, curr, 3);
		break;
	case 4:
		return _mm_alignr_epi8(next, curr, 4);
		break;
	case 5:
		return _mm_alignr_epi8(next, curr, 5);
		break;
	case 6:
		return _mm_alignr_epi8(next, curr, 6);
		break;
	case 7:
		return _mm_alignr_epi8(next, curr, 7);
		break;
	case 8:
		return _mm_alignr_epi8(next, curr, 8);
		break;
	case 9:
		return _mm_alignr_epi8(next, curr, 9);
		break;
	case 10:
		return _mm_alignr_epi8(next, curr, 10);
		break;
	case 11:
		return _mm_alignr_epi8(next, curr, 11);
		break;
	case 12:
		return _mm_alignr_epi8(next, curr, 12);
		break;
	case 13:
		return _mm_alignr_epi8(next, curr, 13);
		break;
	case 14:
		return _mm_alignr_epi8(next, curr, 14);
		break;
	case 15:
		return _mm_alignr_epi8(next, curr, 15);
		break;
	default:
		printf("error! shift_num: %d\n", shift_num);
		exit(1);
		break;
	}
}

__m128i shift_right_sse(__m128i pri_vec, __m128i vec, int shift_num) {
	if (shift_num % 4 == 0)
		return right_alignr_helper(pri_vec, vec, shift_num / 4);

	__m128i carryover;
	__m128i shiftee;
	__m128i mask;

	carryover = right_alignr_helper(pri_vec, vec, shift_num / 4 + 1);
	print128_bit(carryover);
	carryover = _mm_srli_epi64(carryover, (4 - (shift_num % 4)) * 2);
	print128_bit(carryover);

	if (shift_num > 4)
		shiftee = right_alignr_helper(pri_vec, vec, shift_num / 4);
	else
		shiftee = vec;

	shiftee = _mm_slli_epi64(shiftee, (shift_num % 4) * 2);

	return _mm_or_si128(shiftee, carryover);
}

__m128i shift_left_sse(__m128i vec, __m128i next_vec, int shift_num) {
	if (shift_num % 4 == 0)
		return left_alignr_helper(vec, next_vec, shift_num / 4);

	__m128i carryover;
	__m128i shiftee;
	__m128i mask;

	carryover = left_alignr_helper(vec, next_vec, shift_num / 4 + 1);
	print128_bit(carryover);
	carryover = _mm_slli_epi64(carryover, (4 - (shift_num % 4)) * 2);
	print128_bit(carryover);

	if (shift_num > 4)
		shiftee = left_alignr_helper(vec, next_vec, shift_num / 4);
	else
		shiftee = vec;

	shiftee = _mm_srli_epi64(shiftee, (shift_num % 4) * 2);

	return _mm_or_si128(shiftee, carryover);
}
