/*
 * vector_filter.h
 *
 *  Created on: Nov 12, 2013
 *      Author: hxin
 */

#ifndef VECTOR_FILTER_H_
#define VECTOR_FILTER_H_

#ifndef __aligned__
	#define __aligned__ __attribute__((aligned(16)))
#endif

#include <stdint.h>
#include <nmmintrin.h>

__m128i shift_right_sse(__m128i pri_vec, __m128i vec, int shift_num);

#endif /* VECTOR_FILTER_H_ */
