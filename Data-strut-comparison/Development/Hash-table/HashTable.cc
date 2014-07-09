/*
 * HashTable.cc
 *
 *  Created on: Apr 13, 2013
 *      Author: hxin
 */

#include "HashTable.h"
#include "KmerHash.h"
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <sstream>

HashTable::HashTable() {
	loc_ptr = NULL;
	location = NULL;
	kmerLength = 10;
	queryHash = 0;
	loc_pos = 0;
	rawData.allocate(kmerLength);
	patternLength = kmerLength;
	pattern = string(kmerLength, '1');
}

HashTable::HashTable(int kmerLength) {
	loc_ptr = NULL;
	location = NULL;
	this->kmerLength = kmerLength;
	queryHash = 0;
	loc_pos = 0;
	rawData.allocate(this->kmerLength);
	patternLength = kmerLength;
	pattern = string(kmerLength, '1');
}

HashTable::HashTable(string pattern) {
	loc_ptr = NULL;
	location = NULL;
	kmerLength = 0;
	queryHash = 0;
	loc_pos = 0;
	patternLength = pattern.size();
	this->pattern = pattern;

	for (int i = 0; i < pattern.size(); i++) {
		if (pattern[i] == '1')
			kmerLength++;
	}

	rawData.allocate(kmerLength);

}

HashTable::~HashTable() {
	if (loc_ptr != NULL) {
		delete[] loc_ptr;
		loc_ptr = NULL;
	}
	if (location != NULL) {
		delete[] location;
		location = NULL;
	}
	if (hashFile.is_open())
		hashFile.close();
}

//Function for generating the hashtable
void HashTable::generateHash(string refName, string hashName) {
	refDB.loadRefFile(refName);
	hashFile.open(hashName.c_str(),
			ios_base::in | ios_base::out | ios_base::trunc);

	int chromoNum = 0;

	while (refDB.loadChromo(chromoNum)) {
		chromoNum++;

		//reset hash content
		rawData.clear(kmerLength);
		rawData.chrName = refDB.getChromoName();

		int tempLoc = 0;
		int skip = 0;
		string seed = string('N', patternLength);
		string kmer = string('N', kmerLength);

		//processing a chromosome
		do {
			kmer = refDB.getRefSeq(tempLoc, patternLength);

			tempLoc++;

			char bp = kmer[patternLength - 1];

			if (bp == 'N' || bp == 'M' || bp == 'R') {
				skip = patternLength - 1;
				continue;
			}

			if (skip > 0) {
				skip--;
				continue;
			}

			kmer = patternToKmer(kmer);

			int hashVal = kmerHash.calculateHash(kmer);
//				cout << "kmer: " << kmer << endl;
//				cout << "hashVal: " << hashVal << endl;
			rawData.k_mer[hashVal].push_back(tempLoc - 1);

		} while (!kmer.empty());

		cout << rawData.chrName << endl;

		//Printing data
		int loc_ptr = 0;
		hashFile << rawData.chrName << endl;
		hashFile << kmerLength << endl;
		hashFile << pattern << endl;
		for (int i = 0; i < (1 << (kmerLength * 2)); i++) {
			hashFile << loc_ptr << endl;
			loc_ptr += rawData.k_mer[i].size();
		}

		hashFile << loc_ptr << endl;

		hashFile << "====" << endl;

		int verify = 0;

		for (int i = 0; i < (1 << (kmerLength * 2)); i++) {
			for (int j = 0; j < rawData.k_mer[i].size(); j++) {
				hashFile << (rawData.k_mer[i])[j] << " ";
				verify++;
			}

			hashFile << endl;
		}

		assert(verify == loc_ptr);

	}

	refDB.unloadAll();
}

//Functions for loading the hashtable
void HashTable::loadHash(string FileName) {
	if (hashFile.is_open()) {
		hashFile.close();
		hashPos.clear();
	}

	hashFile.open(FileName.c_str());
	string hashMetaName = "." + FileName + ".meta";
	fstream hashMetaFile;

	hashMetaFile.open(hashMetaName.c_str(), ios_base::in);

	if (hashMetaFile.is_open()) {
		unsigned long int tempPos;
		while (hashMetaFile >> tempPos) {
			hashPos.push_back((streampos) tempPos);
		}

	} else {

		hashMetaFile.open(hashMetaName.c_str(),
				ios_base::out | ios_base::trunc);

		string tempString;

		while (hashFile.good()) {
			hashPos.push_back(hashFile.tellg());
			hashMetaFile << (unsigned long int) hashFile.tellg() << endl;
			//get Chromosome name
			getline(hashFile, tempString);

			//get KmerLength
			getline(hashFile, tempString);
			cout << tempString << endl;
			stringstream(tempString) >> kmerLength;

			//getPattern
			getline(hashFile, tempString);

			//get all kmers
			for (int i = 0; i <= 1 << (kmerLength * 2); i++)
				getline(hashFile, tempString);

			//get "===="
			getline(hashFile, tempString);
			cout << tempString << endl;

			//get all Locs
			for (int i = 0; i < 1 << (kmerLength * 2); i++)
				getline(hashFile, tempString);
		}

	}
	hashMetaFile.close();
}

void HashTable::unloadHash() {
	hashPos.clear();
	hashFile.close();
}

bool HashTable::loadChromo(int chromoNum) {
	if (!hashFile.good()) {
		hashFile.clear();
	}

	cout << "chromoSize: " << hashPos.size() << endl;
	cout << "hashPos[0]: " << hashPos[0] << endl;

	if (chromoNum >= hashPos.size())
		return false;

	hashFile.seekg(hashPos[chromoNum], ios::beg);
	getline(hashFile, chromoName);
	cout << "loading chromosome: " << chromoName << endl;

	hashFile >> kmerLength;
	hashFile >> pattern;
	kmerHash.setKmerSize(kmerLength);
	patternLength = pattern.size();

	if (loc_ptr != NULL) {
		delete[] loc_ptr;
		loc_ptr = NULL;
	}
	if (location != NULL) {
		delete[] location;
		location = NULL;
	}

	loc_ptr = new unsigned int[1 << (kmerLength * 2) + 1];

	for (int i = 0; i <= (1 << (kmerLength * 2)); i++)
		hashFile >> loc_ptr[i];

	string thebar;
	hashFile >> thebar;

//	cout << thebar << endl;

	location = new unsigned int[loc_ptr[1 << kmerLength * 2]];
	for (int i = 0; i < loc_ptr[1 << kmerLength * 2]; i++)
		hashFile >> location[i];

	return true;
}

//Functions for alignment
void HashTable::queryKmer(string Kmer) {
	//If the hashtable uses pattern
	Kmer = patternToKmer(Kmer);

	queryHash = kmerHash.calculateHash(Kmer);
	loc_pos = loc_ptr[queryHash];
}

bool HashTable::endLoc() {
	return (loc_pos >= loc_ptr[queryHash + 1]);
}

string HashTable::getKmer() {
	kmerHash.setKmerSize(kmerLength);
	kmerHash.setHashVal(queryHash);
	string kmer = kmerHash.getKmer();
	return kmerToPattern(kmer);
}

unsigned int HashTable::getLoc() {
	return (location[loc_pos++]);
}

unsigned int HashTable::getLocLength() {
	return loc_ptr[queryHash + 1] - loc_ptr[queryHash];
}

bool HashTable::searchLoc(string Kmer, unsigned targetLoc, unsigned int error) {
	Kmer = patternToKmer(Kmer);
	unsigned int tempHash = kmerHash.calculateHash(Kmer);
	int lower_bound = loc_ptr[tempHash];
	int upper_bound = loc_ptr[tempHash + 1] - 1;
	int mid = lower_bound + (upper_bound - lower_bound) / 2;

	while (lower_bound < upper_bound) {
		if (location[mid] <= targetLoc + error
				&& location[mid] >= targetLoc - error)
			return true;
		else if (location[mid] < targetLoc)
			lower_bound = mid + 1;
		else
			upper_bound = mid - 1;
		mid = lower_bound + (upper_bound - lower_bound) / 2;
	}

	if (location[mid] <= targetLoc + error
			&& location[mid] >= targetLoc - error)
		return true;
	else
		return false;
}

//Iterators
void HashTable::setIterBegin() {
	queryHash = 0;
}

void HashTable::iterNext() {
	queryHash++;
}

bool HashTable::iterIsEnd() {
	if (queryHash >= 1 << (kmerLength * 2))
		return true;
	else
		return false;
}

//Functions for analysis
void HashTable::queryKmer() {
	loc_pos = loc_ptr[queryHash];
}

void HashTable::setQueryHash(unsigned int queryHash) {
	this->queryHash = queryHash;
}

unsigned int HashTable::getKmerLength() {
	return kmerLength;
}

unsigned int HashTable::getSeedLength() {
	return patternLength;
}

void HashTable::setLocPos(unsigned int targetLoc) {

//	cout << "targetLoc: " << targetLoc << endl;

	int lower_bound = loc_ptr[queryHash];
	int upper_bound = loc_ptr[queryHash + 1] - 1;
	int mid = lower_bound + (upper_bound - lower_bound) / 2;
//	cout << "mid: " << mid << endl;

	while (lower_bound < upper_bound) {
		if (location[mid] == targetLoc) {
			loc_pos = mid;
			break;
		} else if (location[mid] < targetLoc)
			lower_bound = mid + 1;
		else
			upper_bound = mid - 1;
		mid = lower_bound + (upper_bound - lower_bound) / 2;

	}

//	 cout << "location[" << mid << "]: " << location[mid] << endl;

	if (location[mid] == targetLoc)
		loc_pos = mid + 1;
	else {
		cerr << "failed to set location " << targetLoc << " of Kmer "
				<< kmerHash.getKmer() << endl;
		exit(1);
	}
}

unsigned int HashTable::getLocLength(string Kmer) {
	Kmer = patternToKmer(Kmer);
	unsigned int tempHash = kmerHash.calculateHash(Kmer);
	return (loc_ptr[tempHash + 1] - loc_ptr[tempHash]);
}

void RawHashData::allocate(int kmerLength) {
	k_mer = new vector<unsigned int> [1 << (kmerLength * 2)];
}

void RawHashData::clear(int kmerLength) {
	for (int i = 0; i < (1 << (kmerLength * 2)); i++)
		k_mer[i].clear();
}

string HashTable::patternToKmer(string seed) {
	if (patternLength == kmerLength)
		return seed;
	else {
		string kmer;
		int oneCount = 0;
		for (int i = patternLength - 1; i >= 0; i--) {
			if (pattern[i] == '1') {
				kmer.push_back(seed[i]);
				oneCount++;
			}
		}

		assert(oneCount == kmerLength);
		return kmer;
	}
}

string HashTable::kmerToPattern(string kmer) {
	if (patternLength == kmerLength)
		return kmer;
	else {
		int oneCount = 0;
		string seed(patternLength, '0');
		for (int i = 0; i < patternLength; i++) {
			if (pattern[i] == '1') {
				seed[i] = kmer[oneCount];
				oneCount++;
			}
		}
		assert(oneCount == kmerLength);
		return seed;
	}
}

string HashTable::applyPattern(string seed) {
	if (patternLength == kmerLength)
		return seed;

	assert(seed.length() >= patternLength);

	string patternedSeed(patternLength, '0');
	for (int i = 0; i < patternLength; i++) {
		if (pattern[i] == '1')
			patternedSeed[i] = seed[i];
	}
	return patternedSeed;
}
