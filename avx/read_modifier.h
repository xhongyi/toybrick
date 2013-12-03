/*
 * read_modifier.h
 *
 *  Created on: Nov 18, 2013
 *      Author: hxin
 */

#ifndef READ_MODIFIER_H_
#define READ_MODIFIER_H_

void add_mismatch(char* DNA, int length, int pos, char base);
void add_ins(char* DNA, int length, int pos, char base);
void add_del(char* DNA, int length, int pos, char base);

void add_mismatch_random(char* DNA, int length, int pos);
void add_ins_random(char* DNA, int length, int pos);
void add_del_random(char* DNA, int length, int pos);

#endif /* READ_MODIFIER_H_ */
