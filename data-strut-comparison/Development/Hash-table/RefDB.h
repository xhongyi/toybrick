/*
 * refDB.h
 *
 *  Created on: May 30, 2013
 *      Author: hxin
 */

#ifndef REFDB_H_
#define REFDB_H_

#include <string>
#include <fstream>
#include <vector>

using namespace std;

class RefDB {
public:
	RefDB();
	~RefDB();
	void loadRefFile(string refName);
	bool loadChromo(int chromoNum);
	void unloadAll();

	unsigned int getChromoLength();
	string getChromoName();
	string getRefSeq(int refLoc, int length);
	char getRefBase(int refLoc, int length);

private:
	ifstream refFile;
	vector<streampos> refPos;
	string chromoName;
	string chromosome;
};

#endif /* REFDB_H_ */
