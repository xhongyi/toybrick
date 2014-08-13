/*
 * LongKmerHash.h
 *
 *  Created on: Dec 2, 2012
 *      Author: hxin
 */

#ifndef LONGKMERHASH_H_
#define LONGKMERHASH_H_

#include <iostream>
#include <string>

using namespace std;

class LongKmerHash {
public:
	LongKmerHash();
	LongKmerHash(int KmerSize);
	LongKmerHash(string DNA, int KmerSize);
	~LongKmerHash();
	LongKmerHash(const LongKmerHash& rhs);

	void calculateHash(string DNA);
	void setKmerSize(int size);

	string getKmer() const;

	static bool compare(const LongKmerHash& lhs, const LongKmerHash& rhs);
private:
	int KmerSize;
	static const int HASH_LEVEL = 3;
	static const int HASH_LENGTH = 32;
	unsigned long int hashVal[HASH_LEVEL];
};

#endif
