/*
 * LongHashTable.cc
 *
 *  Created on: Apr 13, 2013
 *      Author: hxin
 */

#include "LongHashTable.h"
#include <fstream>
#include <cstdlib>
#include <cassert>

LongHashTable::LongHashTable() {
	kmerLength = 10;
	KmerHash.setKmerSize(kmerLength);
	chrHashData = map<LongKmerHash, int,
			bool (*)(const LongKmerHash&, const LongKmerHash&)>(
			&LongKmerHash::compare);
}

LongHashTable::LongHashTable(int kmerLength) {
	this->kmerLength = kmerLength;
	KmerHash.setKmerSize(kmerLength);
	chrHashData = map<LongKmerHash, int,
			bool (*)(const LongKmerHash&, const LongKmerHash&)>(
			&LongKmerHash::compare);
}

LongHashTable::~LongHashTable() {
	if (hashFile.is_open())
		hashFile.close();
}

//Function for generating the LongHashTable
void LongHashTable::generateHash(string refName, string hashName,
		int chromoNum) {
	refDB.loadRefFile(refName);
	hashFile.open(hashName.c_str(),
			ios_base::in | ios_base::out | ios_base::trunc);

	refDB.loadChromo(chromoNum);

	int tempLoc = 0;
	int skip = 0;
	string kmer = string('N', kmerLength);

	//processing a chromosome
	do {
		kmer = refDB.getRefSeq(tempLoc, kmerLength);

		tempLoc++;

		makeProgress("Generating long hash", tempLoc,
				(int) refDB.getChromoLength(), (int) 1000);

		char bp = kmer[kmerLength - 1];

		if (bp == 'N' || bp == 'M' || bp == 'R') {
			skip = kmerLength - 1;
			continue;
		}

		if (skip > 0) {
			skip--;
			continue;
		}

		KmerHash.calculateHash(kmer);

		if (chrHashData.find(KmerHash) != chrHashData.end()) {
//			if (kmer == "AAAAAAAAAAAA")
//				cout << tempLoc << " ";
			chrHashData[KmerHash]++;
		}
		else
			chrHashData[KmerHash] = 1;

	} while (!kmer.empty());

	//Printing data
	map<LongKmerHash, int, bool (*)(const LongKmerHash&, const LongKmerHash&)>::iterator iter;
	hashFile << kmerLength << endl;
	for (iter = chrHashData.begin(); iter != chrHashData.end(); iter++) {
		hashFile << iter->first.getKmer() << " " << iter->second << endl;
	}
}

//Functions for loading the LongHashTable
void LongHashTable::loadHash(string FileName) {
	if (hashFile.is_open()) {
		hashFile.close();
		chrHashData.clear();
	}

	hashFile.open(FileName.c_str());

	hashFile >> kmerLength;
	KmerHash.setKmerSize(kmerLength);

	string Kmer;
	int locLen;

	hashFile >> Kmer;
	hashFile >> locLen;

	while (hashFile.good()) {
		chrHashData[LongKmerHash(Kmer, kmerLength)] = locLen;

		hashFile >> Kmer;
		hashFile >> locLen;
	}
}

void LongHashTable::unloadHash() {
	chrHashData.clear();
	hashFile.close();
}

unsigned int LongHashTable::getSeedLength() {
	return kmerLength;
}

unsigned int LongHashTable::getLocLength(string Kmer) {
	assert(Kmer.size() == kmerLength);
	KmerHash.calculateHash(Kmer);
	return chrHashData[KmerHash];
}

void LongHashTable::PrintAnalysis() {
	int seedCompo[4] = { 0, 0, 0, 0 };
	unsigned long int mapCost = 0;
	unsigned long int totalLoc = 0;

	map<LongKmerHash, int, bool (*)(const LongKmerHash&, const LongKmerHash&)>::iterator iter;
	for (iter = chrHashData.begin(); iter != chrHashData.end(); iter++) {
		if (iter->second < 50)
			seedCompo[0]++;
		else if (iter->second < 100)
			seedCompo[1]++;
		else if (iter->second < 1000)
			seedCompo[2]++;
		else
			seedCompo[3]++;

		mapCost += (unsigned long int)iter->second * (unsigned long int)iter->second;
		totalLoc+= (unsigned long int)iter->second;
	}

	cout << "Total mapping cost is: " << mapCost << endl;
	cout << seedCompo[0] << " seeds have 0-50 locations" << endl;
	cout << seedCompo[1] << " seeds have 50-100 locations" << endl;
	cout << seedCompo[2] << " seeds have 100-1000 locations" << endl;
	cout << seedCompo[3] << " seeds have 1000+ locations" << endl;

	cout << "Total location: " << totalLoc << endl;

}

