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
#include <string.h>
#include "popcount.h"
#include "bit_convert.h"
#include "mask.h"

uint8_t MASK_01[32] __aligned__ = { 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
		0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
		0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
		0x55 };

//ED Beginning Mask
uint8_t MASK_SSE_BEG[128] __aligned__
= { 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xf0, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

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
		printf("Error! Invalid shift! From vector_fiter.c: shift_num: %d\n",
				shift_num);
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
		printf("Error! Invalid shift! From vector_fiter.c: shift_num: %d\n",
				shift_num);
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
//	print128_bit(carryover);
	carryover = _mm_srli_epi64(carryover, (4 - (shift_num % 4)) * 2);
//	print128_bit(carryover);

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
//	print128_bit(carryover);
	carryover = _mm_slli_epi64(carryover, (4 - (shift_num % 4)) * 2);
//	print128_bit(carryover);

	if (shift_num > 4)
		shiftee = left_alignr_helper(vec, next_vec, shift_num / 4);
	else
		shiftee = vec;

	shiftee = _mm_srli_epi64(shiftee, (shift_num % 4) * 2);

	return _mm_or_si128(shiftee, carryover);
}

__m128i xor11complement_sse(__m128i input) {
	__m128i temp, result;
	__m128i mask = _mm_load_si128((__m128i *) MASK_01);

	temp = _mm_and_si128(input, mask);
	temp = _mm_slli_epi16(temp, 1);
	result = _mm_or_si128(temp, input);

	mask = _mm_slli_epi16(mask, 1);
	temp = _mm_and_si128(input, mask);
	temp = _mm_srli_epi16(temp, 1);
	result = _mm_or_si128(result, temp);

	return result;
}

#define _MAX_LENGTH_ 320

uint8_t read_bit_t[_MAX_LENGTH_ / 4 + 16] __aligned__;
uint8_t ref_bit_t[_MAX_LENGTH_ / 4 + 16] __aligned__;
//__m128i zero_mask = _mm_set1_epi8(0x00);
//__m128i one_mask = _mm_set1_epi8(0xff);

int bit_vec_filter_m128_sse(uint8_t *read_vec, uint8_t *ref_vec, int length,
		int max_error) {
	const __m128i zero_mask = _mm_set1_epi8(0x00);
	const __m128i one_mask = _mm_set1_epi8(0xff);

	int total_byte = (length - 1) / BYTE_BASE_NUM + 1;

	int total_difference = 0;

	//Start iteration
	int i, j;
	//read data
	__m128i prev_read_XMM = _mm_set1_epi8(0x0);
	__m128i curr_read_XMM = *((__m128i *) (read_bit_t));
	//ref data
	__m128i prev_ref_XMM = _mm_set1_epi8(0x0);
	__m128i curr_ref_XMM = *((__m128i *) (ref_bit_t));

	__m128i read_XMM;
	__m128i ref_XMM;
	__m128i temp_diff_XMM;
	__m128i diff_XMM;
	__m128i mask;
	for (i = 0; i < total_byte; i += SSE_BYTE_NUM) {
//		printf("\niteration: %d\n", i);

		curr_read_XMM = *((__m128i *) (read_bit_t + i));
		curr_ref_XMM = *((__m128i *) (ref_bit_t + i));

		diff_XMM = _mm_xor_si128(curr_read_XMM, curr_ref_XMM);
		diff_XMM = xor11complement_sse(diff_XMM);

		if (i + SSE_BYTE_NUM >= total_byte) {
//			printf("WTF\n");
			if (length % SSE_BASE_NUM) {
				mask = _mm_load_si128(
						(__m128i *) (MASK_SSE_END
								+ (length % SSE_BASE_NUM) * SSE_BYTE_NUM));
//				printf("Mask: \n");
//				print128_bit(mask);
				diff_XMM = _mm_and_si128(mask, diff_XMM);
			}
		}

		for (j = 1; j <= max_error; j++) {
			//Right shift read
			read_XMM = shift_right_sse(prev_read_XMM, curr_read_XMM, j);
			temp_diff_XMM = _mm_xor_si128(read_XMM, curr_ref_XMM);
			temp_diff_XMM = xor11complement_sse(temp_diff_XMM);
			if (i == 0) {
//				mask = shift_right_sse(zero_mask, one_mask, j);
				mask = _mm_load_si128(
						(__m128i *) (MASK_SSE_BEG + (j - 1) * SSE_BYTE_NUM));

				temp_diff_XMM = _mm_and_si128(mask, temp_diff_XMM);
			}
			if (i + SSE_BYTE_NUM >= total_byte) {
				if (length % SSE_BASE_NUM) {
					mask = _mm_load_si128(
							(__m128i *) (MASK_SSE_END
									+ (length % SSE_BASE_NUM) * SSE_BYTE_NUM));
					temp_diff_XMM = _mm_and_si128(mask, temp_diff_XMM);
				}
			}

			diff_XMM = _mm_and_si128(diff_XMM, temp_diff_XMM);

			//Right shift ref
			ref_XMM = shift_right_sse(prev_ref_XMM, curr_ref_XMM, j);
			temp_diff_XMM = _mm_xor_si128(curr_read_XMM, ref_XMM);
			temp_diff_XMM = xor11complement_sse(temp_diff_XMM);
			if (i == 0) {
//				mask = shift_right_sse(zero_mask, one_mask, j);
				mask = _mm_load_si128(
						(__m128i *) (MASK_SSE_BEG + (j - 1) * SSE_BYTE_NUM));

//				printf("Mask: \n");
//				print128_bit(mask);
				temp_diff_XMM = _mm_and_si128(mask, temp_diff_XMM);
			}
			if (i + SSE_BYTE_NUM >= total_byte) {
//				printf("WTF\n");
				if (length % SSE_BASE_NUM) {
					mask = _mm_load_si128(
							(__m128i *) (MASK_SSE_END
									+ (length % SSE_BASE_NUM) * SSE_BYTE_NUM));
//					printf("Mask: \n");
//					print128_bit(mask);
					temp_diff_XMM = _mm_and_si128(mask, temp_diff_XMM);
				}
			}

			diff_XMM = _mm_and_si128(diff_XMM, temp_diff_XMM);
		}

		total_difference += popcount11_m128i_sse(diff_XMM);

		prev_read_XMM = curr_read_XMM;
		prev_ref_XMM = curr_ref_XMM;

		if (total_difference > max_error)
			return 0;
	}

	return 1;
}

int bit_vec_filter_sse(char* read, char* ref, int length, int max_error) {
	//Get ready the bits
//	memcpy(read_t, read, length * sizeof(char));
//	memcpy(ref_t, ref, length * sizeof(char));

	sse3_convert2bit11(read, length, read_bit_t);
	sse3_convert2bit11(ref, length, ref_bit_t);

	return bit_vec_filter_m128_sse(read_bit_t, ref_bit_t, length, max_error);
}

void bit_vec_filter_sse_simulate(char* read, char* ref, int length,
		int max_error, int loc_num) {
	//Get ready the bits
//	memcpy(read_t, read, length * sizeof(char));
//	memcpy(ref_t, ref, length * sizeof(char));

	sse3_convert2bit11(read, length, read_bit_t);
	sse3_convert2bit11(ref, length, ref_bit_t);

	while (loc_num--)
		bit_vec_filter_m128_sse(read_bit_t, ref_bit_t, length, max_error);
}

