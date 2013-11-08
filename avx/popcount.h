/*
 * popcount.h
 *
 *  Created on: Nov 7, 2013
 *      Author: hxin
 */

#ifndef POPCOUNT_H_
#define POPCOUNT_H_

#ifndef __aligned__
	#define __aligned__ __attribute__((aligned(16)))
#endif

#include <stdint.h>

uint32_t ssse3_popcount(uint8_t* buffer, int chunks16);

uint32_t ssse3_popcount11(uint8_t* buffer, int chunks16);

uint32_t builtin_popcount(uint8_t* buffer, int chunks16);

uint32_t popcount(uint8_t *buffer, int chunks16);

uint32_t popcount11(uint8_t *buffer, int chunks16);

#endif /* POPCOUNT_H_ */
