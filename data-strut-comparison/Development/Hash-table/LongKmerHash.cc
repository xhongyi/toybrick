#include "LongKmerHash.h"
#include <cassert>
#include <iostream>
#include <cstdlib>

LongKmerHash::LongKmerHash(){
	KmerSize = 10;
	for (int i = 0; i < HASH_LEVEL; i++)
		hashVal[i] = 0;
}

LongKmerHash::LongKmerHash(int KmerSize) {
	this->KmerSize = KmerSize;
	for (int i = 0; i < HASH_LEVEL; i++)
			hashVal[i] = 0;
}

LongKmerHash::LongKmerHash(string Kmer, int KmerSize) {
	//cout << "KmerSize: " <<  KmerSize << endl;
	assert(KmerSize <= HASH_LEVEL * HASH_LENGTH / 2);
	this->KmerSize = KmerSize;
	for (int i = 0; i < HASH_LEVEL; i++)
		hashVal[i] = 0;
	calculateHash(Kmer);
}

LongKmerHash::~LongKmerHash() {
}

LongKmerHash::LongKmerHash(const LongKmerHash& rhs) {
	this->KmerSize = rhs.KmerSize;
	for (int i = 0; i < HASH_LEVEL; i++)
		hashVal[i] = rhs.hashVal[i];
}

string LongKmerHash::getKmer() const {
	string Kmer(KmerSize, 'N');
	for (int i = 0; i < KmerSize; i++) {
		int hashIdx = i / HASH_LENGTH;
		int hashRemainder = i % HASH_LENGTH;

		int bpVal = (hashVal[hashIdx] >> (2 * hashRemainder)) & 3;
		switch (bpVal) {
		case 0:
			Kmer[KmerSize - i - 1] = 'A';
			break;
		case 1:
			Kmer[KmerSize - i - 1] = 'C';
			break;
		case 2:
			Kmer[KmerSize - i - 1] = 'G';
			break;
		case 3:
			Kmer[KmerSize - i - 1] = 'T';
			break;
		default:
			Kmer[KmerSize - i - 1] = 'N';
			break;
		}
	}
	return Kmer;
}

void LongKmerHash::calculateHash(string Kmer) {
	assert(Kmer.length() == KmerSize);
	assert(KmerSize <= HASH_LEVEL * HASH_LENGTH);
	//Fill different Lv of the hash
	for (int hashLv = 0; hashLv < HASH_LEVEL; hashLv++) {
		hashVal[hashLv] = 0;
		unsigned long bpVal = 0;

		for (int i = 0; i < HASH_LENGTH; i++) {
			if (hashLv * HASH_LENGTH + i >= KmerSize) {
				//cout << "hashVal[" << hashLv << "]: " << hashVal[hashLv] << endl;
				break;
			}
			switch (Kmer[hashLv * HASH_LENGTH + i]) {
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
				cerr << "Wrong bp: " << Kmer[hashLv * HASH_LENGTH + i] << endl;
				cerr << "The Kmer: " << Kmer << endl;
				exit(1);
			}

			hashVal[hashLv] = (hashVal[hashLv] << 2) | bpVal;
		}
	}
}

void LongKmerHash::setKmerSize(int size) {
	KmerSize = size;
}

bool LongKmerHash::compare(const LongKmerHash& lhs, const LongKmerHash& rhs) {
	for (int hashLv = HASH_LEVEL - 1; hashLv >= 0; hashLv--) {
		if (lhs.hashVal[hashLv] != rhs.hashVal[hashLv])
			return (lhs.hashVal[hashLv] < rhs.hashVal[hashLv]);
	}
	return (lhs.hashVal[0] < rhs.hashVal[0]);
}

