/*
 * KmerHash.h
 *
 *  Created on: Dec 2, 2012
 *      Author: hxin
 */

#ifndef KMERHASH_H_
#define KMERHASH_H_

#include <iostream>
#include <string>
#include <vector>

const unsigned int FIRST_LEVEL = 12;

using namespace std;

class KmerHash {
public:
	KmerHash();
	KmerHash(string DNA);
	~KmerHash();
	KmerHash(const KmerHash& rhs);

	//Get kmer
	void setKmerSize(unsigned int kmerSize);
	void setHashVal(unsigned int hashVal);
	string getKmer() const;

	//Calculate Hash
	unsigned int getHash() const;

	vector<unsigned int> getData() const;
	unsigned int calculateHash(string DNA);

//	static bool compare(const KmerHash& lhs, const KmerHash& rhs);
private:
	vector<unsigned int> hashVal;
	unsigned int KmerSize;
};

#endif
