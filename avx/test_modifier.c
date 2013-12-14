#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "read_modifier.h"
#include "vector_filter.h"
using namespace std;

int my_alligner(char* refDNA, char* hongyiDNA, int length, int err) {
	if (memcmp(refDNA, hongyiDNA, length) == 0)
		return 1;
	else
		return 1;
}

int main(int argc, char* argv[]) {
	char * DNA = (char*)malloc(sizeof(char) * 200);
	int i;

	memcpy(DNA, "TCGCTAGTAGCCGGAACTAACAGGTAGGCCTACATCAGCTATACGGCATCGGCAACCTTGAGGGGCCGCGCCCCGTTACACTTTATACGTTTCCCTTGCAAGCCTTCGTGTCGGAGCATATGTATATGG",128);
	srand(time(0));

	if (argc != 5) {
		printf("Usage: %s <size> <e> <injected_error> <function>\n", argv[0]);
		exit(-1);
	}
	int err = atoi(argv[3]);
	int testErr = atoi(argv[2]);
	int size = atoi(argv[1]);
	int func = atoi(argv[4]);
	unsigned long long ret;
	if (func == 0) {
		ret = test_alligner_random(&bit_vec_filter_sse1, DNA, size, testErr, err);
	} else if (func == 1) {
		ret = test_alligner_random(&bit_vec_filter_no_flipping_sse1, DNA, size, testErr, err);
	}

	printf("false positives: %lld\n", ret);
	
/*
	memcpy(DNA,"AAAAAAAAAAAA",13);
	printf("ori: %s\n", DNA);
	add_mis_pos_base(DNA, 12, 5, 'G');
	printf("mis: %s\n", DNA);
	
	memcpy(DNA,"AAAAAAAAAAAA",13);
	printf("ori: %s\n", DNA);
	add_ins_pos_base(DNA, 12, 5, 'G');
	printf("mis: %s\n", DNA);

	memcpy(DNA,"AAAAAAAAAAAA",13);
	printf("ori: %s\n", DNA);
	add_del_pos_base(DNA, 12, 5, 'G');
	printf("mis: %s\n", DNA);

	for (i=0;i<10;i++) {
	memcpy(DNA,"AAAAAAAAAAAA",13);
	printf("ori: %s\n", DNA);
	add_del_pos(DNA, 12, 5);
	printf("mis: %s\n", DNA);
	}
*/

	return 0;
}



