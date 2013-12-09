#include "mask.h"

#ifndef BOOST_PP_IS_ITERATING

uint8_t __MASK_SSE_END_ [SSE_BIT_LENGTH * SSE_BYTE_NUM / BASE_SIZE] = {

#define BOOST_PP_ITERATION_LIMITS	(0, SSE_BIT_LENGTH / 2  - 1)
#define BOOST_PP_FILENAME_1			"mask.c" // this file
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

uint8_t* MASK_SSE_END = __MASK_SSE_END_;

#endif // BOOST_PP_IS_ITERATING

