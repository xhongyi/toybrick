/*
 * bit_convert.c
 *
 *  Created on: Nov 7, 2013
 *      Author: hxin
 */

#include "bit_convert.h"
#include <stdio.h>
#include <xmmintrin.h>
#include <tmmintrin.h>
#include <emmintrin.h>

uint8_t BASE_SHIFT[16] __aligned__ = {0x0, 0x4, 0x8, 0xc, 0x2, 0x6, 0xa, 0xe,
	0x1, 0x5, 0x9, 0xd, 0x3, 0x7, 0xb, 0xf};

char MASKA_16[16] __aligned__ = {'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
	'A', 'A', 'A', 'A', 'A', 'A', 'A'};

char MASKC_16[16] __aligned__ = {'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C',
	'C', 'C', 'C', 'C', 'C', 'C', 'C'};

char MASKG_16[16] __aligned__ = {'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
	'G', 'G', 'G', 'G', 'G', 'G', 'G'};

char MASKT_16[16] __aligned__ = {'T', 'T', 'T', 'T', 'T', 'T', 'T', 'T', 'T',
	'T', 'T', 'T', 'T', 'T', 'T', 'T'};

uint8_t BIT_A_16[16] __aligned__ = {0x00, 0x00, 0x00, 0x00, //A
	0x00, 0x00, 0x00, 0x00,//A
	0x00, 0x00, 0x00, 0x00,//A
	0x00, 0x00, 0x00, 0x00//A
};

uint8_t BIT_C_16[16] __aligned__ = {0x55, 0x55, 0x55, 0x55, //C
	0x55, 0x55, 0x55, 0x55,//C
	0x55, 0x55, 0x55, 0x55,//C
	0x55, 0x55, 0x55, 0x55//C
};

uint8_t BIT_G_16[16] __aligned__ = {0xaa, 0xaa, 0xaa, 0xaa, //G
	0xaa, 0xaa, 0xaa, 0xaa,//G
	0xaa, 0xaa, 0xaa, 0xaa,//G
	0xaa, 0xaa, 0xaa, 0xaa//G
};

uint8_t BIT_T_16[16] __aligned__ = {0xff, 0xff, 0xff, 0xff, //T
	0xff, 0xff, 0xff, 0xff,//T
	0xff, 0xff, 0xff, 0xff,//T
	0xff, 0xff, 0xff, 0xff//T
};

uint8_t LOC_MASK[64] = { 0xc0, 0xc0, 0xc0, 0xc0, //1
		0xc0, 0xc0, 0xc0, 0xc0, //1
		0xc0, 0xc0, 0xc0, 0xc0, //1
		0xc0, 0xc0, 0xc0, 0xc0, //1
		0x0c, 0x0c, 0x0c, 0x0c, //3
		0x0c, 0x0c, 0x0c, 0x0c, //3
		0x0c, 0x0c, 0x0c, 0x0c, //3
		0x0c, 0x0c, 0x0c, 0x0c, //3
		0x30, 0x30, 0x30, 0x30, //2
		0x30, 0x30, 0x30, 0x30, //2
		0x30, 0x30, 0x30, 0x30, //2
		0x30, 0x30, 0x30, 0x30, //2
		0x03, 0x03, 0x03, 0x03, //4
		0x03, 0x03, 0x03, 0x03, //4
		0x03, 0x03, 0x03, 0x03, //4
		0x03, 0x03, 0x03, 0x03 //4
		};

void c_convert2bit(char *str, int length, uint8_t *bits) {
	int i;
	int j;
	int k;

	for (j = 0; j < length * 2 / (8 * sizeof(bits[0])) + 1; j++)
		bits[j] = 0;

	for (i = 0; i < length; i++) {
		j = i * 2 / (sizeof(bits[0]) * 8);
		k = i * 2 % (sizeof(bits[0]) * 8);

		switch (str[i]) {
		case 'C':
			bits[j] += (1ULL << (sizeof(bits[0]) * 8 - 2)) >> k;
			break;
		case 'G':
			bits[j] += (2ULL << (sizeof(bits[0]) * 8 - 2)) >> k;
			break;
		case 'T':
			bits[j] += (3ULL << (sizeof(bits[0]) * 8 - 2)) >> k;
			break;
		default:
			break;
		}
		/*
		 int m;
		 for (m = 63; m >= 0; m--) {
		 //			cout << "m:" << m << " ";
		 if (temp & (1ULL << m) )
		 printf("1");
		 else
		 printf("0");
		 }
		 */
	}
}

void print128_bit(__m128i var) {
	uint8_t *val = (uint8_t*) &var;
	int i;
	for (i = 0; i < 16; i++) {
		int m;
		for (m = 7; m >= 0; m--) {
			if (val[i] & (1ULL << m))
				printf("1");
			else
				printf("0");
		}
	}

	printf("\n");

}

void print128_hex(__m128i var) {
	uint8_t *val = (uint8_t*) &var;
	printf("Numerxcal: %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\n",
			val[0], val[1], val[2], val[3], val[4], val[5], val[6], val[7],
			val[8], val[9], val[10], val[11], val[12], val[13], val[14],
			val[15]);
}

void sse3_convert2bit(char *str, int length, uint8_t *bits) {

	__m128i *shift_hint = (__m128i *) BASE_SHIFT;
	__m128i *cast_str;
//	__m128i *output_str;

	__m128i temp;
	__m128i result;

	//loading comparison values
	__m128i *maskA = (__m128i *) MASKA_16;
	__m128i *maskC = (__m128i *) MASKC_16;
	__m128i *maskG = (__m128i *) MASKG_16;
	__m128i *maskT = (__m128i *) MASKT_16;
	__m128i *mask;

	int i, j;

	for (i = 0; i < length; i += 64) {
		for (j = 0; j < 64; j += 16) {
			cast_str = (__m128i *) (str + i + j);
			*cast_str = _mm_shuffle_epi8(*cast_str, *shift_hint);
		}

		//printf("After shifting 0: %s\n", A_filled_t);

		for (j = 8; j < 64; j += 32) {
			cast_str = (__m128i *) (str + i + j);
			temp = _mm_loadu_si128(cast_str);
			temp = _mm_shuffle_epi32(temp, 0x4e);
			_mm_storeu_si128(cast_str, temp);
		}

		//printf("After shifting 1: %s\n", A_filled_t);

		temp = *( (__m128i *) (str + i + 16) );
		*( (__m128i *) (str + i + 16) ) = *( (__m128i *) (str + i + 32) );
		*( (__m128i *) (str + i + 32) ) = temp;

		//printf("After shifting 2: %s\n", A_filled_t);

		for (j = 0; j < 64; j += 16) {
			cast_str = (__m128i *) (str + i + j);
			temp = _mm_load_si128(cast_str);
			temp = _mm_shuffle_epi32(temp, 0xd8);
			_mm_store_si128(cast_str, temp);
		}

		//printf("After shifting 3: %s\n", A_filled_t);

		for (j = 8; j < 64; j += 32) {
			cast_str = (__m128i *) (str + i + j);
			temp = _mm_loadu_si128(cast_str);
			temp = _mm_shuffle_epi32(temp, 0x4e);
			_mm_storeu_si128(cast_str, temp);
		}

		result = _mm_set1_epi32(0);
		__m128i* bit_idx = (__m128i*) (bits + (i * 2) / (8 * sizeof(bits[0]) ) );
		*bit_idx = _mm_set1_epi32(0);
		for (j = 0; j < 64; j += 16) {
			cast_str = (__m128i *) (str + i + j);
			temp = _mm_cmpeq_epi8(*maskC, *cast_str);
			result = _mm_and_si128(temp, *((__m128i *) BIT_C_16));
			//print128_hex(result);

			temp = _mm_cmpeq_epi8(*maskG, *cast_str);
			temp = _mm_and_si128(temp, *((__m128i *) BIT_G_16));
			result = _mm_or_si128(result, temp);
			//print128_hex(result);

			temp = _mm_cmpeq_epi8(*maskT, *cast_str);
			temp = _mm_and_si128(temp, *((__m128i *) BIT_T_16));
			result = _mm_or_si128(result, temp);
			//print128_hex(result);

			mask = (__m128i *) (LOC_MASK + j);

			result = _mm_and_si128(*mask, result);
			//print128_hex(result);
			*bit_idx = _mm_or_si128(*bit_idx, result);
			//print128_bit(result);
			//print128_bit(*bit_idx);

		}
	}
}

