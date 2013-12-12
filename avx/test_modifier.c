#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_modifier.h"
using namespace std;

int my_alligner(char* refDNA, char* hongyiDNA, int length, int err) {
	if (memcmp(refDNA, hongyiDNA, length) == 0)
		return 1;
	else
		return 1;
}

int main(int argc, char* argv[]) {
	char * DNA = (char*)malloc(sizeof(char) * 13);
	int i;

	memcpy(DNA,"AAAAAAAAAAAA",13);
	printf("ori: %s\n", DNA);
	int err = (argc > 1) ? atoi(argv[1]) : 1;
	int ret = test_alligner_random(&my_alligner, DNA, 12, err);
	printf("nerr of %d: %d\n", err, ret);
	
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



