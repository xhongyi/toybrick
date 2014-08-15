/*
 * SeedTree.h
 *
 *  Created on: May 28, 2013
 *      Author: Hongyi
 */

#ifndef SeedTree_H_
#define SeedTree_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "RefDB.h"
#include "HashTable.h"
#include <math.h>
#include <map>

#define LOG_BASE 2

using namespace std;

class TreeNode {
public:
	TreeNode();
	TreeNode(string seed);
	TreeNode(string seed, int locNum);
	~TreeNode();

	//set member variables
	void setSeed(string seed);
	void setLocNum(int locNum);
	void setLoc(int index, unsigned int loc);
	void addLoc(unsigned int loc);
	void setA(TreeNode* A);
	void setC(TreeNode* C);
	void setT(TreeNode* T);
	void setG(TreeNode* G);
	void deleteChildren();

	//modify memrber varibales. Returns false if content not found
	void deleteLoc(unsigned int loc);

	//get member variables
	string getSeed();
	int getSeedLength();
	int getLocNum();
	unsigned int getLoc(int index);
	bool isEmpty();
	const vector<unsigned int>& getLocList();
	TreeNode * getA();
	TreeNode * getC();
	TreeNode * getT();
	TreeNode * getG();

	TreeNode ** getPtrA();
	TreeNode ** getPtrC();
	TreeNode ** getPtrT();
	TreeNode ** getPtrG();

	//check if node is leaf
	bool isLeaf();

private:
	string seed;
	vector<unsigned int> locList;
	TreeNode* A;
	TreeNode* C;
	TreeNode* T;
	TreeNode* G;
};

struct NodeMeta {
	NodeMeta(TreeNode* node, unsigned int index) {
		this->node = node;
		this->index = index;
	}
	;

	TreeNode* node;
	unsigned int index;
};

struct HetAnaData {
	HetAnaData() {
		effectiveSeedLen = 0;
		locationLen = 0;
		extEntryNum = 0;
	}
	;

	int effectiveSeedLen;
	int locationLen;
	int extEntryNum;
};

class SeedTree {
public:

	SeedTree();
	~SeedTree();

	void loadFiles(string hashFileName, string refFileName);
	void loadChromo(int chromoNum);

	//Get effective length
	int getEffectiveLen(string longSeed);
	HetAnaData getHetAnaData(string longSeed);
	int getShortLocationLen(string shortSeed);
	int getRootSeedLength();

	void genTree(unsigned int threshold, unsigned int maxSeedLen);
	void genTree(unsigned int threshold, unsigned int maxSeedLen,
			string treeFile);

	void evalHetSeed();
	void evalHetSeed(int maxLv);
	void checkRoots();
	//Caution: This changes the tree structure
	void evalRepeatCoverage();
	void prunePartialRepeats(int lowerBound);
	void pruneGenuineRepeats(int lowerBound);
	void analyse(double percentage);

	void printRootNodes();
	void printRootBranch(string rootSeed);
	void printLeafNodes();
	void printAnalysisData();
	void printEvaluationData(int entryThreshold = 100);
	void printRepeatCoverage();

	void genTreeFake();

	void genXml(string directoryPath);

	void loadTree(string treeFile);

private:
	//GenTree
	void genTreeHelper(vector<TreeNode*> &parentNodes,
			vector<TreeNode*> &childrenNodes, unsigned int seedLen);
	void genTreeHelper(vector<NodeMeta> &parentNodes,
			vector<NodeMeta> &childrenNodes, unsigned int seedLen,
			unsigned int curIdx, ofstream &treeFile);

	//Probe tree
	TreeNode* getLeafNode(const string& longSeed, int curIdx,
			TreeNode* curNode);

	//PruneTree
	void removeLocListInBranch(string endSeed, const vector<unsigned int>& loc,
			const int offset);

	//Dumpxml
	void dumpXml(TreeNode * root, string parent, string base);
	void writeHeader();
	void startEdge();
	void addNode(string label, string loc, string loc_log);
	int appendFile(string edgeFile);
	void addEdge(string target, string source, string base);

	//Gather tree data. Analysis.
	void analyseBreadth(double percentage, vector<TreeNode*> &LeafNodes,
			bool analysis, int maxDepth = 1000);
	void analyseBreadthHelper(vector<TreeNode*> &parentNodes,
			vector<TreeNode*> &childrenNodes, double percentage,
			int availableDepth, vector<TreeNode*> &LeafNodes, bool analysis);

	//Evaluation
	void evalEntries();

	//Print Analysis Data
	void printExpSeedNum();
	void printSeedCompo();
	void printSplit();

	//Print Evaluation Data
	void printEntryCostCompo();
	void printEntryCost();
	void printEntrySizeCompo();
	void printEntries(int entryThreshold = 0);

	//SeedTree data
	unsigned int expThreshold;
	unsigned int maxSeedLen;
	unsigned int rootSeedLength;
	unsigned int rootKmerLength;

	//HashTable signatures
	unsigned long int cheapSeedCost;
	unsigned long int expensiveSeedCost;

	//Evaluation results
	unsigned long int hetSeedCost;

	//Analysis data
	vector<unsigned int> expNum;
	vector<map<unsigned int, unsigned int> > seedLvs;
	vector<unsigned int> evenSplits;
	vector<TreeNode*> trueLeafNodes;
	vector<TreeNode*> truncLeafNodes;
	vector<unsigned int> repeatCoverages;

//Root data
	vector<TreeNode*> rootNodes;
	vector<vector<TreeNode*> > rootEntries;

	//Generate helper data
	RefDB ref;
	HashTable ht;

	//Dump data
	ofstream nfile, efile;
};

#endif
