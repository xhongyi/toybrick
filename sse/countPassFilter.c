/*
 * vector_filterMain.c
 *
 *  Created on: Nov 12, 2013
 *      Author: hxin
 */

//#ifndef BOOST_PP_IS_ITERATING
//#include "print.h"
#include <stdint.h>
#include "vector_filter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mask.h"

char read[128];
char ref[128];

char read_t[128] __aligned__ = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
char ref_t[128] __aligned__ = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

int main(int argc, char* argv[]) {
	
	if (argc != 4) {
		printf("Usage: $>bin input output error\n");
		exit(1);
	}

	int error = atoi(argv[3]);

//	FILE *input;
	FILE *output;
//	input = fopen(argv[1], "r");
	output = fopen(argv[2], "w");
	
	size_t length;
	char* tempstr = NULL;

	int passNum = 0;
	int totalNum = 0;

//	while (getline(&tempstr, &length, input) != -1) {
	while (read_t != "end_of_file") {
		scanf(tempstr);
		tempstr[strlen(tempstr) - 1] = '\0';
		strncpy(read_t, tempstr, lengkkth);
		strncpy(read, tempstr, length);
//		getline(&tempstr, &length, input);
		scanf(tempstr);
		tempstr[strlen(tempstr) - 1] = '\0';
		strncpy(ref_t, tempstr, length);
		strncpy(ref, tempstr, length);

		if (bit_vec_filter_sse1(read_t, ref_t, length, error)) {
			fprintf(output, "%s\n", read);
			fprintf(output, "%s\n", ref);
			passNum++;
		}

		totalNum++;

	}

	printf("passNum:\t%d\n", passNum);
	printf("totalNum:\t%d\n", totalNum);

	fclose(input);
	fclose(output);

	return 0;

}

//#endif
