#include "mask.h"



/*
 * Mask for 1 bit bases
 */

uint8_t __MASK_SSE_END1_ [SSE_BIT_LENGTH * SSE_BYTE_NUM / BASE_SIZE1] = {

#define BOOST_PP_ITERATION_LIMITS	(0, SSE_BIT_LENGTH - 1)
#define BOOST_PP_FILENAME_1			"1masks.hpp" // this file
#include BOOST_PP_ITERATE()

};

#undef BOOST_PP_ITERATION_LIMITS
#undef BOOST_PP_FILENAME_1

///*
// * Mask for 2 bit bases
// */

uint8_t __MASK_SSE_END11_ [SSE_BIT_LENGTH * SSE_BYTE_NUM / BASE_SIZE11] = {

#define BOOST_PP_ITERATION_LIMITS	(0, SSE_BIT_LENGTH / 2  - 1)
#define BOOST_PP_FILENAME_1			"11masks.hpp" // this file
#include BOOST_PP_ITERATE()

};

uint8_t* MASK_SSE_END1 = __MASK_SSE_END1_;
uint8_t* MASK_SSE_END11 = __MASK_SSE_END11_;

