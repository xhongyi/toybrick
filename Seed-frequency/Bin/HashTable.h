/*
 * HashTable.h
 *
 *  Created on: Apr 7, 2013
 *      Author: hxin
 */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#define KMER_LENGTH 10

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "KmerHash.h"
#include "RefDB.h"

using namespace std;

//misc class
class RawHashData {
public:
	RawHashData() {
		k_mer = NULL;
	}
	;
	RawHashData(int kmerLength) {
		k_mer = new vector<unsigned int> [1 << (kmerLength * 2)];
	}
	;
	~RawHashData() {
		if (k_mer != NULL) {
			delete[] k_mer;
			k_mer = NULL;
		}
	}
	;
	void allocate(int kmerLength);
	void clear(int kmerLength);
	string chrName; //Chromosome Name;
	vector<unsigned int>* k_mer;
};

class HashTable {
public:
	//Generator and Destructor
	HashTable();
	HashTable(int kmerLength);
	HashTable(string pattern);
	~HashTable();

	//Function for generating the hashtable
	void generateHash(string RefName, string FileName);

	//Functions for loading the hashtable
	void loadHash(string Filename);
	void unloadHash();
	bool loadChromo(int chromoNum);

	//Functions for alignment
	void queryKmer(string Kmer);
	bool endLoc();
	string getKmer();
	//returns the location and move the internal indexer to the next one
	unsigned int getLoc();
	unsigned int getLocLength();
	bool searchLoc(string Kmer, unsigned targetLoc, unsigned int error);

	//Iterators
	void setIterBegin();
	void iterNext();
	bool iterIsEnd();

	//Functions for analysis
	void queryKmer(); //Needed when iterating HashTable
	void setQueryHash(unsigned int queryHash); //Not used yet. Not suggested with pattern
	unsigned int getKmerLength();
	unsigned int getSeedLength();
	void setLocPos(unsigned int location);
	unsigned int getLocLength(string Kmer);

	//Functions for calculating pattern;
	string patternToKmer(string seed);
	string kmerToPattern(string kmer);
	string applyPattern(string seed);

	//void printStat();

private:
	//raw data
	KmerHash kmerHash;
	RawHashData rawData;

	//loading data
	fstream hashFile;
	vector<streampos> hashPos;

	//HashTable signature
	unsigned int patternLength;
	string pattern;
	string chromoName;
	unsigned int kmerLength;
	unsigned int* loc_ptr;
	unsigned int* location;

	//mapping data
	unsigned int queryHash;
	unsigned int loc_pos;

	//refDB in hashGen
	RefDB refDB;
};
#endif /* HASHTABLE_H_ */
