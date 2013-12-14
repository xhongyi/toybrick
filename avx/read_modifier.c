/*
 * read_modifier.c
 *
 *  Created on: Nov 18, 2013
 *      Author: hxin
 */
#include "read_modifier.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <algorithm>

#define MAX_TEST_MINS 60
#define ALIGNER_SPEED 200000000.0
#define MAX_PRINT_DNA 1000

int printed_DNA;

//char * internal_DNA = NULL;
//int internal_length = 0;
//
//void check_length(int length) {
//	if (length > internal_length) {
//		if (internal_DNA != NULL)
//			free(internal_DNA);
//
//		internal_DNA = malloc(sizeof(char) * length);
//		internal_length = length;
//	}
//}

//srand(time(0));

int test_alligner_random(int (*fAlligner)(char *, char *, int, int), char* DNA, int length, int testErr, int err) {
	char* _refDNA = DNA;
	char* _modDNA = (char*)malloc(sizeof(char) * length);
	if (_modDNA == NULL) {
		printf("malloc failed!!!\n");
		exit(0);
	}
	memcpy(_modDNA, DNA, length);

	// Estimate time
	double estimated_time;
	int e;
	for (e=1; e<=err; e++) {
		estimated_time = pow((12.0*length), (double)e)/ALIGNER_SPEED;
		if (estimated_time > (double)MAX_TEST_MINS) {
			e--;
			estimated_time = pow((12.0*length), (double)e)/ALIGNER_SPEED;
			break;
		}
	}

	// Estimate iterations needed
	if (err <= e) {
#ifndef NO_WARNING
		printf("Using exhaustive test!!!\n");
#endif
		int ret = test_alligner_exhaust(fAlligner, DNA, length, testErr, err);
		free(_modDNA);
		return ret;
	}
	int iterations = (int)((double)MAX_TEST_MINS/estimated_time);
#ifndef NO_WARNINGa
	printf("Using random test!!!\n");
	printf("%d iterations needed...\n", iterations);
	printf("Each iteration takes about %4.1f mins, total %4.1f mins\n", estimated_time, estimated_time*iterations);
#endif

	// Random tests
	int ret = 0;
	for (int i=0;i<iterations;i++) {
		add_n_any(_modDNA, length, err-e);
		ret += test_alligner_exhaust_helper(fAlligner, _refDNA, _modDNA, length, testErr, err, e);
		memcpy(_modDNA, _refDNA, length);
	}
	free(_modDNA);
	return ret;
}

int test_alligner_exhaust_helper(int (*fAlligner)(char *, char *, int, int), char* refDNA, char* modDNA, int length, int testErr, int totErr, int err) {
	if (err == 0) {
		char* _refDNA = (char*)malloc(sizeof(char) * std::max(129,length+1));
		char* _modDNA = (char*)malloc(sizeof(char) * std::max(129,length+1));
		memcpy(_refDNA, refDNA, length);
		memcpy(_modDNA, modDNA, length);
		int ret = !(*fAlligner)(_refDNA, _modDNA, length, testErr);
		if (ret && (printed_DNA < MAX_PRINT_DNA)) {
			printed_DNA++;
			memcpy(_modDNA, modDNA, length);
			_modDNA[length] = '\0';
			printf("Err: %s\n", _modDNA);
		}
		free(_refDNA);
		free(_modDNA);
		return ret;
	} else {
		char* _curModDNA = modDNA;
		char* _nextModDNA = (char*)malloc(sizeof(char) * length);
		int ret = 0;
		for (int i = 0; i < length; i++) {

			memcpy(_nextModDNA, _curModDNA, length);
			add_mis_pos_base(_nextModDNA, length, i, 'A');
			ret += test_alligner_exhaust_helper(fAlligner, refDNA, _nextModDNA, length, testErr, totErr, err-1);

			memcpy(_nextModDNA, _curModDNA, length);
			add_mis_pos_base(_nextModDNA, length, i, 'G');
			ret += test_alligner_exhaust_helper(fAlligner, refDNA, _nextModDNA, length, testErr, totErr, err-1);

			memcpy(_nextModDNA, _curModDNA, length);
			add_mis_pos_base(_nextModDNA, length, i, 'C');
			ret += test_alligner_exhaust_helper(fAlligner, refDNA, _nextModDNA, length, testErr, totErr, err-1);

			memcpy(_nextModDNA, _curModDNA, length);
			add_mis_pos_base(_nextModDNA, length, i, 'T');
			ret += test_alligner_exhaust_helper(fAlligner, refDNA, _nextModDNA, length, testErr, totErr, err-1);

			memcpy(_nextModDNA, _curModDNA, length);
			add_ins_pos_base(_nextModDNA, length, i, 'A');
			ret += test_alligner_exhaust_helper(fAlligner, refDNA, _nextModDNA, length, testErr, totErr, err-1);

			memcpy(_nextModDNA, _curModDNA, length);
			add_ins_pos_base(_nextModDNA, length, i, 'G');
			ret += test_alligner_exhaust_helper(fAlligner, refDNA, _nextModDNA, length, testErr, totErr, err-1);

			memcpy(_nextModDNA, _curModDNA, length);
			add_ins_pos_base(_nextModDNA, length, i, 'C');
			ret += test_alligner_exhaust_helper(fAlligner, refDNA, _nextModDNA, length, testErr, totErr, err-1);

			memcpy(_nextModDNA, _curModDNA, length);
			add_ins_pos_base(_nextModDNA, length, i, 'T');
			ret += test_alligner_exhaust_helper(fAlligner, refDNA, _nextModDNA, length, testErr, totErr, err-1);

			memcpy(_nextModDNA, _curModDNA, length);
			add_del_pos_base(_nextModDNA, length, i, 'A');
			ret += test_alligner_exhaust_helper(fAlligner, refDNA, _nextModDNA, length, testErr, totErr, err-1);

			memcpy(_nextModDNA, _curModDNA, length);
			add_del_pos_base(_nextModDNA, length, i, 'G');
			ret += test_alligner_exhaust_helper(fAlligner, refDNA, _nextModDNA, length, testErr, totErr, err-1);

			memcpy(_nextModDNA, _curModDNA, length);
			add_del_pos_base(_nextModDNA, length, i, 'C');
			ret += test_alligner_exhaust_helper(fAlligner, refDNA, _nextModDNA, length, testErr, totErr, err-1);

			memcpy(_nextModDNA, _curModDNA, length);
			add_del_pos_base(_nextModDNA, length, i, 'T');
			ret += test_alligner_exhaust_helper(fAlligner, refDNA, _nextModDNA, length, testErr, totErr, err-1);
		}
		free(_nextModDNA);
		return ret;
	}
}

int test_alligner_exhaust(int (*fAlligner)(char *, char *, int, int), char* DNA, int length, int testErr, int err) {
	char* _refDNA = DNA;
	char* _modDNA = (char*)malloc(sizeof(char) * length);
	if (_modDNA == NULL) {
		printf("malloc failed!!!\n");
		exit(0);
	}
	memcpy(_modDNA, _refDNA, length);
#ifndef NO_WARNING
	double iterations = pow(12.0 * length, (double)err);
	if (iterations)
		printf("Warning: testing for %4.0f iterations, estimated runtime %4.1f mins!!!\n", iterations, iterations/ALIGNER_SPEED);
#endif
	int ret = test_alligner_exhaust_helper(fAlligner, _refDNA, _modDNA, length, testErr, err, err);
	free(_modDNA);
	return ret;
}

void add_n_any(char* DNA, int length, int n) {
	for (int i=0;i<n;i++) {
		switch (rand()%3) {
			case 0:
				add_mis_pos_base(DNA, length, rand()%length, get_rand_base());
				break;
			case 1:
				add_ins_pos_base(DNA, length, rand()%length, get_rand_base());
				break;
			case 2:
				add_del_pos_base(DNA, length, rand()%length, get_rand_base());
				break;
		}
	}
}

void add_n_mis(char* DNA, int length, int n) {
	for (int i=0;i<n;i++) {
		add_mis_pos_base(DNA, length, rand()%length, get_rand_base());
	}
}

void add_n_ins(char* DNA, int length, int n) {
	for (int i=0;i<n;i++) {
		add_ins_pos_base(DNA, length, rand()%length, get_rand_base());
	}
}

void add_n_del(char* DNA, int length, int n) {
	for (int i=0;i<n;i++) {
		add_del_pos_base(DNA, length, rand()%length, get_rand_base());
	}
}

void add_mis_pos_base(char* DNA, int length, int pos, char base) {
	DNA[pos] = base;
}

void add_ins_pos_base(char* DNA, int length, int pos, char base) {
	memcpy(DNA + pos + 1, DNA + pos, length - pos - 1);
	DNA[pos] = base;
}

void add_del_pos_base(char* DNA, int length, int pos, char base) {
	memcpy(DNA + pos, DNA + pos + 1, length - pos - 1);
	DNA[length - 1] = base;
}

void add_mis_pos(char* DNA, int length, int pos) {
	add_mis_pos_base(DNA, length, pos, get_rand_base());
}

void add_ins_pos(char* DNA, int length, int pos) {
	add_ins_pos_base(DNA, length, pos, get_rand_base());
}

void add_del_pos(char* DNA, int length, int pos) {
	add_del_pos_base(DNA, length, pos, get_rand_base());
}

char get_rand_base() {
	char randBase = rand() % 4;
	switch (randBase) {
		case 0:
			return 'A';
		case 1:
			return 'G';
		case 2:
			return 'C';
		case 3:
			return 'T';
	}
	// Should never reach here
	assert(true);
	return 'A';
}
