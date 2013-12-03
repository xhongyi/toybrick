/*
 * read_modifier.c
 *
 *  Created on: Nov 18, 2013
 *      Author: hxin
 */
#include <stdio.h>

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

void add_mismatch(char* DNA, int length, int pos, char base) {
	DNA[pos] = base;
}

void add_ins(char* DNA, int length, int pos, char base) {
	memcpy(DNA + pos + 1, DNA + pos, length - pos - 1);
	DNA[pos] = base;
}

void add_del(char* DNA, int length, int pos, char base) {
	memcpy(DNA + pos, DNA + pos + 1, length);
	DNA[length - 1] = base;
}

void add_mismatch_random(char* DNA, int length, int pos) {

}
