#ifndef __MASK_H__
#define __MASK_H__

#include <stdint.h>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/iteration.hpp>
#include <boost/preprocessor/arithmetic.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

#define SSE_BIT_LENGTH		128
#define BASE_SIZE			2
#define SSE_BASE_NUM		BOOST_PP_DIV(SSE_BIT_LENGTH, BASE_SIZE)
#define BYTE_BASE_NUM		BOOST_PP_DIV(8, BASE_SIZE)
#define SSE_BYTE_NUM		BOOST_PP_DIV(SSE_BIT_LENGTH, 8)

extern uint8_t *MASK_SSE_END;

/*
= {

#define BOOST_PP_ITERATION_LIMITS	(0, SSE_BIT_LENGTH / 2  - 1)
#define BOOST_PP_FILENAME_1			"mask.h" // this file
#include BOOST_PP_ITERATE()

#else // BOOST_PP_IS_ITERATING

#define I		BOOST_PP_ITERATION()
#define PRINT_DATA(z, n, data) data

#define FF_NUM	BOOST_PP_DIV(I, BYTE_BASE_NUM)
		BOOST_PP_ENUM(FF_NUM, PRINT_DATA, 0xff)

#if		FF_NUM != 0
		BOOST_PP_COMMA()
#endif	//FF_NUM != 0

#if		BOOST_PP_MOD(I, BYTE_BASE_NUM) == 1
		0x03
#elif	BOOST_PP_MOD(I, BYTE_BASE_NUM) == 2
		0x0f
#elif	BOOST_PP_MOD(I, BYTE_BASE_NUM) == 3
		0x3f
#else
		0x00
#endif	//End of switch

#define ZZ_NUM	BOOST_PP_SUB( BOOST_PP_SUB(SSE_BYTE_NUM, 1), FF_NUM)

#if		ZZ_NUM != 0
		BOOST_PP_COMMA()
#endif	//ZZ_NUM != 0

		BOOST_PP_ENUM(ZZ_NUM, PRINT_DATA, 0x00)

#if I != BOOST_PP_ITERATION_FINISH()
		BOOST_PP_COMMA()
#endif // I != BOOST_PP_ITERATION_FINISH()

#undef	FF_NUM
#undef	ZZ_NUM
#undef	I
#undef	PRINT_DATA

#endif // BOOST_PP_IS_ITERATING
#ifndef BOOST_PP_IS_ITERATING
};
*/

#endif // __MASK_H__
