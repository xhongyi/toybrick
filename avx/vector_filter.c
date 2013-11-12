/*
 * vector_ed.c
 *
 *  Created on: Nov 8, 2013
 *      Author: hxin
 */

#include "vector_ed.h"
#include <nmmintrin.h>

__m128i shift_right_sse(__m128i pri_vec, __m128i vec, int shift_num) {
	if (shift_num % 4 == 0)
		return __m128i _mm_aligner_epi8(pri_vec, vec, 16 - shift_num / 4);

	__m128i carryover;
	__m128i shiftee;
	__m128i mask;

	carryover = _mm_aligner_epi8(pri_vec, vec, 15 - shift_num / 4);
	carryover = _mm_slli_epi64(carryover, (4 - shift_num % 4) * 2);

	if (shift_num > 4)
		shiftee = _mm_aligner_epi8(pri_vec, vec, 15 - shift_num / 4);
	else
		shiftee = vec;

	shiftee = _mm_srli_epi64(shiftee, (shift_num % 4) * 2);

	return _mm_or_si128(shiftee, carryover);
}
