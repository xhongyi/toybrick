/*
 * SRS.h
 *
 *  Created on: Nov 7, 2013
 *      Author: hxin
 */

#ifndef SRS_H_
#define SRS_H_

#ifndef __aligned__
	#define __aligned__ __attribute__((aligned(16)))
#endif

#include <stdint.h>
#include <nmmintrin.h>

uint32_t SRS_m128i_sse(__m128i reg);

uint32_t hamming_m128i_sse(__m128i reg);

uint32_t SRS_sse(uint8_t* buffer, int chunks16);

uint32_t builtin_popcount(uint8_t* buffer, int chunks16);

uint32_t SRS(uint8_t *buffer, int chunks16);

#endif /* SRS_H_ */
