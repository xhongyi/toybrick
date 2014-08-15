/*
 * LongHashTable.h
 *
 *  Created on: Apr 7, 2013
 *      Author: hxin
 */

#ifndef LongHashTable_H_
#define LongHashTable_H_

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include "LongKmerHash.h"
#include "RefDB.h"

using namespace std;

class LongHashTable {
public:
	//Generator and Destructor
	LongHashTable();
	LongHashTable(int kmerLength);
	~LongHashTable();

	//Function for generating the LongHashTable
	void generateHash(string RefName, string HashName, int chromoNum);

	//Functions for loading the LongHashTable
	void loadHash(string Filename);
	void unloadHash();

	unsigned int getSeedLength();
	unsigned int getLocLength(string Kmer);

	void PrintAnalysis();

private:
	//raw data
	LongKmerHash KmerHash;

	//loading data
	fstream hashFile;
	map<LongKmerHash, int, bool (*)(const LongKmerHash&, const LongKmerHash&)> chrHashData;

	//LongHashTable signature
	string chromoName;
	unsigned int kmerLength;

	//refDB in hashGen
	RefDB refDB;
};

template<class T>
void makeProgress(string jobName, T curProgress, const T fullSize,
		const T granularity) {
	if (curProgress % granularity == 0) {
		cerr << '\r' << jobName << ": "
				<< (unsigned int) ((double) curProgress / fullSize * 100) << "%"
				<< "\tcurProgress: " << curProgress << "    ";
		cerr.flush();
		//curProgress = 0;
	}
}

#endif /* LongHashTable_H_ */
