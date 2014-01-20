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

char read_t[128] __aligned__;// = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
char ref_t[128] __aligned__;// = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

int main(int argc, char* argv[]) {
	
/*	if (argc != 4) {
		printf("Usage: $>bin input output error\n");
		exit(1);
	}
*/
	if (argc != 2) {
		printf("Usage: $>bin error\n");
		exit(1);
	}

//	int error = atoi(argv[3]);
	int error = atoi(argv[1]);

//	FILE *input;
//	FILE *output;
//	input = fopen(argv[1], "r");
//	output = fopen(argv[2], "w");

	size_t lineLength;	
	size_t length;
	char* tempstr = NULL;

	long long unsigned int passNum = 0;
	long long unsigned int totalNum = 0;

//	while (getline(&tempstr, &length, input) != -1) {
	do {
		getline(&tempstr, &lineLength, stdin);
		tempstr[strlen(tempstr) - 1] = '\0';
		if (strcmp(tempstr, "end_of_file\0") == 0)
				break;
		length = strlen(tempstr);
		if (length > 128)
			length = 128;
		strncpy(read_t, tempstr, length);
		strncpy(read, tempstr, length);
//		getline(&tempstr, &length, input);
		getline(&tempstr, &lineLength, stdin);
		tempstr[strlen(tempstr) - 1] = '\0';
		strncpy(ref_t, tempstr, length);
		strncpy(ref, tempstr, length);
//		printf("lineLength: %d, length: %lu\n", lineLength, length);
//		printf("read_t:\t%s\nref_t:\t%s\n", read_t, ref_t);

		if (bit_vec_filter_sse1(read_t, ref_t, length, error)) {
			printf("%s\n", read);
			printf("%s\n", ref);
			passNum++;
		}

		totalNum++;
	} while (1);

	printf("end_of_file\n");
	fprintf(stderr, "passNum:\t%lld\n", passNum);
	fprintf(stderr, "totalNum:\t%lld\n", totalNum);


//	fclose(input);
//	fclose(output);

	return 0;

}

//#endif
