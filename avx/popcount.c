/*
 * popcount.c
 *
 *  Created on: Nov 7, 2013
 *      Author: hxin
 */
#include "popcount.h"
#include <nmmintrin.h>

#ifdef DEBUG
#include <assert.h>
#endif

uint8_t POPCOUNT_4bit[16] __aligned__ = {
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

uint8_t POPCOUNT_4bit11[16] __aligned__ = {
/* 0 */0,
/* 1 */0,
/* 2 */0,
/* 3 */1,
/* 4 */0,
/* 5 */0,
/* 6 */0,
/* 7 */1,
/* 8 */0,
/* 9 */0,
/* a */0,
/* b */1,
/* c */1,
/* d */1,
/* e */1,
/* f */2 };

uint32_t ssse3_popcount_core(uint8_t* buffer, int chunks16, uint8_t *map) {
	static char MASK_4bit[16] = {0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf};

	uint32_t result;

	__asm__ volatile ("movdqu (%%eax), %%xmm7" : : "a" (map));
	__asm__ volatile ("movdqu (%%eax), %%xmm6" : : "a" (MASK_4bit));
	__asm__ volatile ("pxor    %%xmm5, %%xmm5" : : ); // xmm5 -- global accumulator

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
		}
		else {
			k = chunks16;
			chunks16 = 0;
		}
#undef MAX

		__asm__ volatile ("pxor %xmm4, %xmm4"); // xmm4 -- local accumulator
		for (n=0; n < k; n++) {
			__asm__ volatile(
				"movdqa	  (%%eax), %%xmm0	\n"
				"movdqa    %%xmm0, %%xmm1	\n"

				"psrlw         $4, %%xmm1	\n"
				"pand      %%xmm6, %%xmm0	\n"	// xmm0 := lower nibbles
				"pand      %%xmm6, %%xmm1	\n"	// xmm1 := higher nibbles

				"movdqa    %%xmm7, %%xmm2	\n"
				"movdqa    %%xmm7, %%xmm3	\n"	// get popcount
				"pshufb    %%xmm0, %%xmm2	\n"	// for all nibbles
				"pshufb    %%xmm1, %%xmm3	\n"	// using PSHUFB

				"paddb     %%xmm2, %%xmm4	\n"	// update local
				"paddb     %%xmm3, %%xmm4	\n"	// accumulator

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
		"movd      %%xmm0, %%eax	\n"
		: "=a" (result)
	);

	return result;
}

uint32_t ssse3_popcount(uint8_t* buffer, int chunks16) {
	return ssse3_popcount_core(buffer, chunks16, POPCOUNT_4bit);
}

uint32_t ssse3_popcount11(uint8_t* buffer, int chunks16) {
	return ssse3_popcount_core(buffer, chunks16, POPCOUNT_4bit11);
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

uint32_t popcount(uint8_t *buffer, int chunks16) {
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

uint32_t popcount11(uint8_t *buffer, int chunks16) {
	uint32_t result = 0;

	int i;
	for (i = 0; i < chunks16 * 16; i++) {
		int j;
		uint8_t mask = 3;
		for (j = 0; j < 4; j++) {
			printf("%x ", mask);
			if ( (buffer[i] & mask) == mask)
				result++;
			mask = mask << 2;
		}
	}

	return result;
}
