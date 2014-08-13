/*
 * RefDB.cc
 *
 *  Created on: May 30, 2013
 *      Author: hxin
 */
#include "RefDB.h"
#include <cstdlib>
#include <fstream>
#include <iostream>

RefDB::RefDB() {
}

RefDB::~RefDB() {
}

void RefDB::loadRefFile(string refName) {
	if (refFile.is_open()) {
		refFile.close();
		refPos.clear();
	}

	refFile.open(refName.c_str());

	if (!refFile.is_open()) {
		cerr << "RefFile open failed" << endl;
		exit(1);
	}

	string refMetaName = "." + refName + ".meta";
	fstream refMetaFile(refMetaName.c_str(), ios_base::in);

	if (refMetaFile.is_open()) {
		unsigned long int tempPos;
		while (refMetaFile >> tempPos) {
			refPos.push_back((streampos) tempPos);
		}

		cout << "refPos.size(): " << refPos.size() << endl;

	} else {

		refMetaFile.open(refMetaName.c_str(), ios_base::out | ios_base::trunc);

		string temp;
		streampos tempRefPos;

		while (refFile.good()) {
			tempRefPos = refFile.tellg();
			getline(refFile, temp);
			if (temp[0] == '>') {
				refPos.push_back(tempRefPos);
				refMetaFile << (unsigned long int) tempRefPos << endl;
			}
		}
	}

	refMetaFile.close();
}

bool RefDB::loadChromo(int chromoNum) {
	cout << "RefDB loading chromosome: " << chromoNum << endl;

	if (chromoNum >= refPos.size())
		return false;

	chromosome.clear();
	refFile.clear();
	refFile.seekg(refPos[chromoNum]);

	getline(refFile, chromoName);

	string temp;

	while (refFile.good() && (temp.size() == 0 || temp[0] != '<')) {
		chromosome.append(temp);
		getline(refFile, temp);
		if (temp[0] == '>') {
			cout << "stop!" << endl;
			break;
		}
	}

	return true;
}

void RefDB::unloadAll() {
	refPos.clear();
	chromosome.clear();
	refFile.close();
}

unsigned int RefDB::getChromoLength() {
	return chromosome.size();
}

string RefDB::getChromoName() {
	return chromoName;
}

string RefDB::getRefSeq(int refLoc, int length) {
	//If oversize, return empty seed.
	if (refLoc >= chromosome.size())
		return string("");

	if (refLoc + length > chromosome.size())
		return (chromosome.substr(refLoc, length)
				+ string(refLoc + length - chromosome.size(), 'N'));

	return chromosome.substr(refLoc, length);
}

char RefDB::getRefBase(int refLoc, int length) {
	if (refLoc + length > chromosome.size())
		return 'N';
	return chromosome[refLoc + length - 1];
}
