/*
 * popcount.c
 *
 *  Created on: Nov 7, 2013
 *      Author: hxin
 */
#include "SRS.h"
#include <stdio.h>
#include <nmmintrin.h>
#include "mask.h"

#ifdef DEBUG
#include <assert.h>
#endif

uint8_t HAMMING_TABLE[16] __aligned__ = {
/* 0 */0,
/* 1 */1,
/* 2 */1,
/* 3 */2,
/* 4 */1,
/* 5 */2,
/* 6 */2,
/* 7 */3,
/* 8 */1,
/* 9 */2,
/* a */2,
/* b */3,
/* c */2,
/* d */3,
/* e */3,
/* f */4 };

uint8_t SRS_TABLE[16] __aligned__ = {
/* 0 */0,
/* 1 */1,
/* 2 */1,
/* 3 */1,
/* 4 */1,
/* 5 */2,
/* 6 */2,
/* 7 */1,
/* 8 */1,
/* 9 */2,
/* a */2,
/* b */2,
/* c */1,
/* d */2,
/* e */1,
/* f */1 };

uint32_t SRS_core(uint8_t* buffer, int chunks16, uint8_t *map) {
	
	uint32_t result;

	__asm__ volatile ("movdqu (%%rax), %%xmm7" : : "a" (map));
	__asm__ volatile ("movdqu (%%rax), %%xmm6" : : "a" (MASK_0F));
	__asm__ volatile ("pxor    %%xmm5, %%xmm5" : : );
	// xmm5 -- global accumulator

	result = 0;

	int k, n, i;

	i = 0;
	while (chunks16 > 0) {
		// max(POPCOUNT_8bit) = 8, thus byte-wise addition could be done
		// for floor(255/8) = 31 iterations
#define MAX 32
		if (chunks16 > MAX) {
			k = MAX;
			chunks16 -= MAX;
		} else {
			k = chunks16;
			chunks16 = 0;
		}
#undef MAX

		__asm__ volatile ("pxor %xmm4, %xmm4");
		// xmm4 -- local accumulator
		for (n = 0; n < k; n++) {
			__asm__ volatile(
					"movdqa	  (%%rax), %%xmm0	\n"
					"movdqa    %%xmm0, %%xmm1	\n"

					"psrlw         $4, %%xmm1	\n"
					"pand      %%xmm6, %%xmm0	\n"	// xmm0 := lower nibbles
					"pand      %%xmm6, %%xmm1	\n"// xmm1 := higher nibbles

					"movdqa    %%xmm7, %%xmm2	\n"
					"movdqa    %%xmm7, %%xmm3	\n"// get popcount
					"pshufb    %%xmm0, %%xmm2	\n"// for all nibbles
					"pshufb    %%xmm1, %%xmm3	\n"// using PSHUFB

					"paddb     %%xmm2, %%xmm4	\n"// update local
					"paddb     %%xmm3, %%xmm4	\n"// accumulator

					:
					: "a" (&buffer[i])
			);
			i += 16;
		}

		// update global accumulator (two 32-bits counters)
		__asm__ volatile (
				"pxor	%xmm0, %xmm0		\n"
				"psadbw	%xmm0, %xmm4		\n"
				"paddd	%xmm4, %xmm5		\n"
		);
	}

	// finally add together 32-bits counters stored in global accumulator
	__asm__ volatile (
			"movhlps   %%xmm5, %%xmm0	\n"
			"paddd     %%xmm5, %%xmm0	\n"
			"movd      %%xmm0, %%rax	\n"
			: "=a" (result)
	);

	return result;
}

uint32_t SRS_m128_core(__m128i reg, uint8_t *map) {

	uint32_t result;

	__asm__ volatile ("movdqu (%%rax), %%xmm7" : : "a" (map));
	__asm__ volatile ("movdqu (%%rax), %%xmm6" : : "a" (MASK_0F));
	// xmm5 -- global accumulator

	result = 0;

	__asm__ volatile ("pxor %xmm4, %xmm4");
	// xmm4 -- local accumulator

	__asm__ volatile(
			"movdqa        %1, %%xmm0	\n"
			"movdqa    %%xmm0, %%xmm1	\n"

			"psrlw         $4, %%xmm1	\n"
			"pand      %%xmm6, %%xmm0	\n"// xmm0 := lower nibbles
			"pand      %%xmm6, %%xmm1	\n"// xmm1 := higher nibbles

			"movdqa    %%xmm7, %%xmm2	\n"
			"movdqa    %%xmm7, %%xmm3	\n"// get popcount
			"pshufb    %%xmm0, %%xmm2	\n"// for all nibbles
			"pshufb    %%xmm1, %%xmm3	\n"// using PSHUFB

			"paddb     %%xmm2, %%xmm4	\n"// update local
			"paddb     %%xmm3, %%xmm4	\n"// accumulator

			"pxor	%%xmm0, %%xmm0		\n"
			"psadbw	%%xmm0, %%xmm4		\n"
			"movhlps   %%xmm4, %%xmm0	\n"
			"paddd     %%xmm4, %%xmm0	\n"
			"movd   %%xmm0, %%rax		\n"
			: "=a" (result)
			: "x" (reg)
	);

	return result;
}

uint32_t hamming_m128i_sse(__m128i reg) {
	return SRS_m128_core(reg, HAMMING_TABLE);
}

uint32_t SRS_m128i_sse(__m128i reg) {
	return SRS_m128_core(reg, SRS_TABLE);
}

uint32_t SRS_sse(uint8_t* buffer, int chunks16) {
	return SRS_core(buffer, chunks16, SRS_TABLE);
}

uint32_t builtin_popcount(uint8_t* buffer, int chunks16) {
	uint32_t result = 0;

	int i;
	for (i = 0; i < chunks16; i++) {
		uint64_t *cast_int64 = (uint64_t*) (buffer + i * 16);
		result += _mm_popcnt_u64(*cast_int64);
		cast_int64 = (uint64_t*) (buffer + i * 16 + 8);
		result += _mm_popcnt_u64(*cast_int64);
	}

	return result;
}

uint32_t SRS(uint8_t *buffer, int chunks16) {
	uint32_t result = 0;

	int i;
	for (i = 0; i < chunks16 * 16; i++) {
		int j;
		uint8_t mask = 1;
		for (j = 0; j < 8; j++) {
			if (buffer[i] & mask)
				result++;
			mask = mask << 1;
		}
	}

	return result;
}

