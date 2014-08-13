#include "KmerHash.h"
#include <cassert>
#include <iostream>
#include <cstdlib>

KmerHash::KmerHash() {
	KmerSize = 10;
	hashVal.clear();
}

KmerHash::KmerHash(string Kmer) {
	//cout << "KmerSize: " <<  KmerSize << endl;
	hashVal.clear();
	calculateHash(Kmer);
}

KmerHash::~KmerHash() {
	hashVal.clear();
}

KmerHash::KmerHash(const KmerHash& rhs) {
	KmerSize = rhs.KmerSize;
	hashVal = rhs.hashVal;
}

void KmerHash::setKmerSize(unsigned int kmerSize) {
	this->KmerSize = kmerSize;
}

void KmerHash::setHashVal(unsigned int hashVal) {
	this->hashVal.clear();
	this->hashVal.push_back(hashVal);
}

string KmerHash::getKmer() const {
	string Kmer(KmerSize, 'N');

	unsigned int hashLv = 0;
	unsigned int hashThresh = FIRST_LEVEL;
	unsigned int previousThresh = 0;
	for (int i = 0; i < KmerSize; i++) {

		int bpVal = (hashVal[hashLv] >> (2 * (i - previousThresh))) & 3;
		switch (bpVal) {
		case 0:
			Kmer[KmerSize - 1 - i] = 'A';
			break;
		case 1:
			Kmer[KmerSize - 1 - i] = 'C';
			break;
		case 2:
			Kmer[KmerSize - 1 - i] = 'G';
			break;
		case 3:
			Kmer[KmerSize - 1 - i] = 'T';
			break;
		default:
			break;

		}

		if (i == hashThresh) {
			hashLv++;
			previousThresh = i;
			hashThresh += sizeof(hashVal[hashLv]);
		}

	}
	return Kmer;
}

unsigned int KmerHash::calculateHash(string Kmer) {
	//Clear hashVal
	hashVal.clear();
	//Fill different Lv of the hash
	unsigned long bpVal = 0;
	KmerSize = Kmer.size();

	unsigned int tempHashVal = 0;
	int hashThresh = FIRST_LEVEL;

	for (int i = 0; i < Kmer.length(); i++) {
		switch (Kmer[i]) {
		case 'A':
			bpVal = 0;
			break;
		case 'C':
			bpVal = 1;
			break;
		case 'G':
			bpVal = 2;
			break;
		case 'T':
			bpVal = 3;
			break;
		case 'N':
		default:
			cerr << "Wrong bp: " << Kmer[i] << endl;
			cerr << "The Kmer: " << Kmer << endl;
			exit(1);
		}

		tempHashVal = (tempHashVal << 2) | bpVal;
		if (i == hashThresh && i != Kmer.length() - 1) {
			hashVal.push_back(tempHashVal);
			tempHashVal = 0;
			hashThresh += sizeof(hashVal[0]) * 4;
		}
	}

	hashVal.push_back(tempHashVal);
	return hashVal[0];
}

vector<unsigned int> KmerHash::getData() const {
	vector<unsigned int> temp(hashVal.begin() + 1, hashVal.end());
	return temp;
}

//bool KmerHash::compare(const KmerHash& lhs, const KmerHash& rhs) {
//	return (lhs.hashVal < rhs.hashVal);
//}

unsigned int KmerHash::getHash() const {
	return hashVal[0];
}
