/*
 * seed_hotspots.cc
 *
 * 	 Created on: May 30, 2013
 *       Author: John Emmons
 * 
 *	Decsription: this code will take a read, split it into evenly spaced intervals,
 *				 and determine how may possible locations each interval (seed) 
 *				 could be mapped. 
 */

#include "SeedTree.h"
#include "HashTable.h"
#include "LongHashTable.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <cassert>

using namespace std;

int main(int argc, char* argv[]) {

	if (argc != 3) {
		cerr	<< endl << "usage: $> ./seed_hotspots <hashFile.fasta.ht.*> <readInPlainText>"
				<< endl;
		exit(1);
	}

	/*
	 * setup the hash table with the reference genome hash table file.
	 * this file should be generated using LongHasTableMain.cc.
	 */
	LongHashTable longht;
	longht.loadHash(argv[1]);

	/*
	 * read in the read from the second CLI argument and record the size
	 */
	string read = argv[2];
	const int readLen = read.length();

	/* 
	 * seed length is an integer for the length of the current seed. The seed is given 
	 * from the has table.
	 */
	const int seedLen = longht.getSeedLength();

	/*
	 * locCompo is a vector that stores the numebr of possible mappings for a given seed.
	 * note: the seeds are NON-OVERLAPPING.
	 */
	vector<long int> locCompo;
	locCompo.resize(read.length() / seedLen, 0);

	/*
	 * loop through the intervals (seeds) in the read
	 */
	for (int i = 0; i < locCompo.size(); i++) {
		// get the i^{th} seed (sub string) from the read
		string seed = read.substr(seedLen * i, seedLen);
	
		// record the number of possible mappings
		locCompo[i] = longht.getLocLength(seed);
	}

	for (int i = 0; i < locCompo.size(); i++) {
		cout << locCompo[i] << "\t";
	}
	cout << locCompo[locCompo.size() - 1] << "\t";

	return 0;
}
