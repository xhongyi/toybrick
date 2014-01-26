#include "emulate_vector.h"

vector_filter::vector_filter() {
	bit_vector = NULL;
	error = 0;
	min_matching = 0;
	length = 0;
};

vector_filter::
