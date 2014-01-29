/*
 * vector_filterMain.c
 *
 *  Created on: Nov 12, 2013
 *      Author: hxin
 */

//#ifndef BOOST_PP_IS_ITERATING
//#include "print.h"
#include <stdint.h>
#include "emulate_vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char read[128];
char ref[128];

int main(int argc, char* argv[]) {
	
	if (argc != 3) {
		printf("Usage: $>bin error min_match\n");
		exit(1);
	}

	int error = atoi(argv[1]);
	int min_match = atoi(argv[2]);

	vector_filter emulator(error, min_match);

	size_t lineLength;
	size_t length;
	char* tempstr = NULL;

	long long unsigned int passNum = 0;
	long long unsigned int totalNum = 0;

	do {
		getline(&tempstr, &lineLength, stdin);
		tempstr[strlen(tempstr) - 1] = '\0';
		if (strcmp(tempstr, "end_of_file\0") == 0)
				break;
		length = strlen(tempstr);
		if (length > 128)
			length = 128;
		strncpy(read, tempstr, length);
		getline(&tempstr, &lineLength, stdin);
		tempstr[strlen(tempstr) - 1] = '\0';
		strncpy(ref, tempstr, length);

		emulator.set_read(read);
		emulator.set_ref(ref);	
		if (emulator.check_match() ) {
			printf("%s\n", read);
			printf("%s\n", ref);
			passNum++;
		}

		totalNum++;
	} while (1);

	printf("end_of_file\n");
	fprintf(stderr, "passNum:\t%lld\n", passNum);
	fprintf(stderr, "totalNum:\t%lld\n", totalNum);

	return 0;

}

//#endif
