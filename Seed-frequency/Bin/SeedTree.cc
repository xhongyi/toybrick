#include "SeedTree.h"
#include <sstream>
#include <cassert>
#include <stdlib.h>
#include <algorithm>

bool compareNodePtr(TreeNode* lhs, TreeNode* rhs) {
	return lhs->getSeed() < rhs->getSeed();
}

//For class TreeNode
TreeNode::TreeNode() {

	A = NULL;
	G = NULL;
	T = NULL;
	C = NULL;
}

TreeNode::TreeNode(string seed) {
	setSeed(seed);
	A = NULL;
	G = NULL;
	T = NULL;
	C = NULL;
}

TreeNode::TreeNode(string seed, int locNum) {
	setSeed(seed);
	setLocNum(locNum);
	A = NULL;
	C = NULL;
	G = NULL;
	T = NULL;
}

TreeNode::~TreeNode() {
	if (A != NULL) {
		delete A;
		A = NULL;
	}
	if (C != NULL) {
		delete C;
		C = NULL;
	}
	if (T != NULL) {
		delete T;
		T = NULL;
	}
	if (G != NULL) {
		delete G;
		G = NULL;
	}
}

void TreeNode::setSeed(string seed) {
	this->seed = seed;
}

void TreeNode::setLocNum(int locNum) {
	locList.resize(locNum);
}

void TreeNode::setLoc(int index, unsigned int loc) {
	locList[index] = loc;
}

void TreeNode::addLoc(unsigned int loc) {
	locList.push_back(loc);
}

const vector<unsigned int>& TreeNode::getLocList() {
	return locList;
}

void TreeNode::setA(TreeNode *A) {
	this->A = A;
}

void TreeNode::setC(TreeNode *C) {
	this->C = C;
}

void TreeNode::setT(TreeNode *T) {
	this->T = T;
}

void TreeNode::setG(TreeNode *G) {
	this->G = G;
}

void TreeNode::deleteChildren() {
	delete A;
	A = NULL;
	delete C;
	C = NULL;
	delete T;
	T = NULL;
	delete G;
	G = NULL;
}

void TreeNode::deleteLoc(unsigned int loc) {
	vector<unsigned int>::iterator delIter = lower_bound(locList.begin(),
			locList.end(), loc);

	if (delIter == locList.end() || *delIter != loc) {
//		cout << "Can't not find location " << loc << " in seed " << seed
//				<< endl;
		return;
	} else
		locList.erase(delIter);
}

string TreeNode::getSeed() {
	return seed;
}

int TreeNode::getSeedLength() {
	return seed.length();
}

int TreeNode::getLocNum() {
	return locList.size();
}

unsigned int TreeNode::getLoc(int index) {
	return locList[index];
}

bool TreeNode::isEmpty() {
	return locList.empty();
}

TreeNode * TreeNode::getA() {
	return A;
}

TreeNode * TreeNode::getC() {
	return C;
}

TreeNode * TreeNode::getT() {
	return T;
}

TreeNode * TreeNode::getG() {
	return G;
}

TreeNode ** TreeNode::getPtrA() {
	return &A;
}

TreeNode ** TreeNode::getPtrC() {
	return &C;
}

TreeNode ** TreeNode::getPtrT() {
	return &T;
}

TreeNode ** TreeNode::getPtrG() {
	return &G;
}

bool TreeNode::isLeaf() {
	if (A != NULL && A->getLocNum() == 0) {
		delete A;
		A = NULL;
	}

	if (C != NULL && C->getLocNum() == 0) {
		delete C;
		C = NULL;
	}

	if (T != NULL && T->getLocNum() == 0) {
		delete T;
		T = NULL;
	}

	if (G != NULL && G->getLocNum() == 0) {
		delete G;
		G = NULL;
	}

	return (A == NULL && C == NULL && T == NULL && G == NULL);
}

//For class SeedTree
SeedTree::SeedTree() {
	expThreshold = 0;
	maxSeedLen = 0;
	rootSeedLength = 0;
	rootKmerLength = 0;

	expensiveSeedCost = 0;
	cheapSeedCost = 0;

	hetSeedCost = 0;
}

SeedTree::~SeedTree() {
//Free all memory of the tree
	while (!rootNodes.empty()) {
		TreeNode* temp = rootNodes.back();
		if (temp != NULL)
			delete temp;
		rootNodes.pop_back();
	}
}

void SeedTree::loadFiles(string hashFileName, string refFileName) {
	ht.loadHash(hashFileName);
	ref.loadRefFile(refFileName);
}

void SeedTree::loadChromo(int chromoNum) {
	ht.loadChromo(chromoNum);
	ref.loadChromo(chromoNum);
}

int SeedTree::getEffectiveLen(string longSeed) {
	//Indicating Jigsaw seed has failed.
	if (longSeed.size() < rootSeedLength)
		return -1;

	string base = ht.applyPattern(longSeed.substr(0, rootSeedLength));
	TreeNode treeNode(base);

	//Locate the root
	vector<TreeNode*>::iterator tempIter = lower_bound(rootNodes.begin(),
			rootNodes.end(), &treeNode, compareNodePtr);

	//return if no root is found, a cheap seed
	if (tempIter == rootNodes.end())
		return rootSeedLength;
	//An expensive seed
	else
		return getLeafNode(longSeed, rootSeedLength, *tempIter)->getSeedLength();
}

HetAnaData SeedTree::getHetAnaData(string longSeed) {
	HetAnaData returnData;
	if (longSeed.size() < rootSeedLength) {
		returnData.effectiveSeedLen = -1;
		returnData.locationLen = -1;
		returnData.extEntryNum = -1;
		return returnData;
	}

	string base = ht.applyPattern(longSeed.substr(0, rootSeedLength));

	TreeNode treeNode(base);

	//Locate the root
	vector<TreeNode*>::iterator tempIter = lower_bound(rootNodes.begin(),
			rootNodes.end(), &treeNode, compareNodePtr);

	//return if no root is found, a cheap seed
	if (tempIter == rootNodes.end()) {
		returnData.effectiveSeedLen = rootSeedLength;
		returnData.locationLen = ht.getLocLength(base);
		returnData.extEntryNum = 0;
		return returnData;
	}
	//An expensive seed
	else {
		TreeNode* treePtr = getLeafNode(longSeed, rootSeedLength, *tempIter);
		returnData.effectiveSeedLen = treePtr->getSeedLength();
		returnData.locationLen = treePtr->getLocNum();
		returnData.extEntryNum =
				rootEntries[tempIter - rootNodes.begin()].size();
		assert (returnData.extEntryNum > 0);
		return returnData;
	}
}

int SeedTree::getShortLocationLen(string shortSeed) {
	assert(shortSeed.length() == rootSeedLength);

	return ht.getLocLength(shortSeed);

}

TreeNode* SeedTree::getLeafNode(const string& longSeed, int curIdx,
		TreeNode* curNode) {

	//The seed is a tail seed and it's not long enough.
	if (curIdx == longSeed.size())
		return curNode;

	TreeNode* nextNode;

	switch (longSeed[curIdx]) {
	case 'A':
		nextNode = curNode->getA();
		break;
	case 'C':
		nextNode = curNode->getC();
		break;
	case 'G':
		nextNode = curNode->getG();
		break;
	case 'T':
		nextNode = curNode->getT();
		break;
	default:
		cerr << "Wrong seed!!" << endl;
		exit(1);
		break;
	}

	//If no further leaf is found, currentIdx will be the leaf length
	if (nextNode == NULL)
		return curNode;
	//Otherwise, recursive call.
	else
		return getLeafNode(longSeed, curIdx + 1, nextNode);
}

int SeedTree::getRootSeedLength() {
	return rootSeedLength;
}

//Generate tree without storing
void SeedTree::genTree(unsigned int threshold, unsigned int maxSeedLen) {
	rootSeedLength = ht.getSeedLength();
	rootKmerLength = ht.getKmerLength();
	expThreshold = threshold;
	this->maxSeedLen = maxSeedLen;

	vector<TreeNode*> parentNodes;
	vector<TreeNode*> childrenNodes;

	ht.setIterBegin();

	while (!ht.iterIsEnd()) {
		if (ht.getLocLength() >= threshold) {
			TreeNode* tempNode = new TreeNode(ht.getKmer(), ht.getLocLength());

			ht.queryKmer();
			int index = 0;
			while (!ht.endLoc()) {
				tempNode->setLoc(index, ht.getLoc());
				index++;
			}

			assert(index == ht.getLocLength());
			rootNodes.push_back(tempNode);
			parentNodes.push_back(tempNode);

			//Calculate expensive seeds' cost
			expensiveSeedCost += (unsigned long int)ht.getLocLength() * (unsigned long int)ht.getLocLength();
		} else {
			//Calculate cheap seeds' cost
			cheapSeedCost += ht.getLocLength() * ht.getLocLength();
		}

		ht.iterNext();
	}

	rootEntries.resize(rootNodes.size());

	genTreeHelper(parentNodes, childrenNodes, rootSeedLength + 1);

	assert(rootEntries.size() == rootNodes.size() );
	for (int i = 0; i < rootEntries.size(); i++)
		assert(rootEntries[i].size() > 0);
}

void SeedTree::genTreeHelper(vector<TreeNode*> &parentNodes,
		vector<TreeNode*> &childrenNodes, unsigned int seedLen) {

	//Meta data for leaf nodes
	TreeNode treeNode;
	vector<TreeNode*>::iterator tempIter;

	//If max depth or if no more expensive seeds
	if (seedLen >= maxSeedLen || parentNodes.empty()) {
		for (int i = 0; i < parentNodes.size(); i++) {
			string baseSeed = ht.applyPattern(
					parentNodes[i]->getSeed().substr(0, rootSeedLength));
			treeNode.setSeed(baseSeed);
			tempIter = lower_bound(rootNodes.begin(), rootNodes.end(),
					&treeNode, compareNodePtr);

			//No expensive root seed found, exits
			assert(
					tempIter != rootNodes.end()
							|| (*tempIter)->getSeed() == baseSeed);

			rootEntries[tempIter - rootNodes.begin()].push_back(parentNodes[i]);
		}
		return;
	}

	childrenNodes.clear();

	vector<TreeNode*> childrenPtr(4, NULL);
	vector<TreeNode**> parentPtr(4, NULL);

	for (int i = 0; i < parentNodes.size(); i++) {

		TreeNode* parent = parentNodes[i];

		TreeNode* A = new TreeNode(parent->getSeed() + 'A');
		TreeNode* C = new TreeNode(parent->getSeed() + 'C');
		TreeNode* T = new TreeNode(parent->getSeed() + 'T');
		TreeNode* G = new TreeNode(parent->getSeed() + 'G');

		for (int j = 0; j < parent->getLocNum(); j++) {
			unsigned int loc = parent->getLoc(j);
			char base = ref.getRefBase(loc, seedLen);

			switch (base) {
			case 'A':
				A->addLoc(loc);
				break;
			case 'C':
				C->addLoc(loc);
				break;
			case 'T':
				T->addLoc(loc);
				break;
			case 'G':
				G->addLoc(loc);
				break;
			default:
				break;
			}
		}

		childrenPtr[0] = A;
		childrenPtr[1] = C;
		childrenPtr[2] = T;
		childrenPtr[3] = G;

		parentPtr[0] = parent->getPtrA();
		parentPtr[1] = parent->getPtrC();
		parentPtr[2] = parent->getPtrT();
		parentPtr[3] = parent->getPtrG();

		for (int j = 0; j < childrenPtr.size(); j++) {

			if (childrenPtr[j]->isEmpty()) {
				*parentPtr[j] = NULL;
				delete childrenPtr[j];
			} else {
				*parentPtr[j] = childrenPtr[j];
				if (childrenPtr[j]->getLocNum() >= expThreshold)
					childrenNodes.push_back(childrenPtr[j]);

				//This is a leaf node
				else {
					string baseSeed = ht.applyPattern(
							childrenPtr[j]->getSeed().substr(0,
									rootSeedLength));
					treeNode.setSeed(baseSeed);
					tempIter = lower_bound(rootNodes.begin(), rootNodes.end(),
							&treeNode, compareNodePtr);

					//No expensive root seed found, exits
					assert(
							tempIter != rootNodes.end()
									|| (*tempIter)->getSeed() == baseSeed);

					rootEntries[tempIter - rootNodes.begin()].push_back(
							childrenPtr[j]);
				}

			}

		}
		/*
		 if (A->isEmpty()) {
		 parent->setA(NULL);
		 delete A;
		 } else {
		 parent->getRefA() = (A);
		 if (A->getLocNum() >= expThreshold)
		 childrenNodes.push_back(A);
		 }

		 if (C->isEmpty()) {
		 parent->setC(NULL);
		 delete C;
		 } else {
		 parent->getRefC() = (C);
		 if (C->getLocNum() >= expThreshold)
		 childrenNodes.push_back(C);
		 }

		 if (T->isEmpty()) {
		 parent->setT(NULL);
		 delete T;
		 } else {
		 parent->getRefT() = (T);
		 if (T->getLocNum() >= expThreshold)
		 childrenNodes.push_back(T);
		 }

		 if (G->isEmpty()) {
		 parent->setG(NULL);
		 delete G;
		 } else {
		 parent->getRefG() = (G);
		 if (G->getLocNum() >= expThreshold)
		 childrenNodes.push_back(G);
		 }
		 */

	}

	genTreeHelper(childrenNodes, parentNodes, seedLen + 1);
}

void SeedTree::genTree(unsigned int threshold, unsigned int maxSeedLen,
		string treeFileName) {
	expThreshold = threshold;
	this->maxSeedLen = maxSeedLen;
	rootSeedLength = ht.getSeedLength();
	rootKmerLength = ht.getKmerLength();

	vector<NodeMeta> parentNodes;
	vector<NodeMeta> childrenNodes;
	unsigned int curIdx = 0;

	ofstream treeFile;
	treeFile.open(treeFileName.c_str());

	ht.setIterBegin();

	while (!ht.iterIsEnd()) {
		if (ht.getLocLength() >= threshold) {

			TreeNode* tempNode = new TreeNode(ht.getKmer(), ht.getLocLength());

			ht.queryKmer();
			treeFile << tempNode->getSeed() << endl;

			int index = 0;
			while (!ht.endLoc()) {
				tempNode->setLoc(index, ht.getLoc());

				treeFile << tempNode->getLoc(index); // Writing location list
				treeFile << " ";

				index++;
			}
			treeFile << "-2" << endl;	// To denote end of location list

			treeFile << "-1" << endl;// Its parent is Supernode, "-1" is for Supernode

			assert(index == ht.getLocLength());
			rootNodes.push_back(tempNode);
			parentNodes.push_back(NodeMeta(tempNode, curIdx));
			curIdx++;

			//Calculate expensive seeds' cost
			expensiveSeedCost += ht.getLocLength() * ht.getLocLength();

		} else {
			//Calculate cheap seeds' cost
			cheapSeedCost += ht.getLocLength() * ht.getLocLength();
		}

		ht.iterNext();
	}

//sort(rootNodes.begin(), rootNodes.end(), compareNodePtr);

	genTreeHelper(parentNodes, childrenNodes, rootSeedLength + 1, curIdx,
			treeFile);
	treeFile << "end" << endl;
	treeFile << expThreshold << endl;
	treeFile << maxSeedLen << endl;
	treeFile << cheapSeedCost << endl;
	treeFile << expensiveSeedCost << endl;
	treeFile << rootSeedLength << endl;
	treeFile << rootKmerLength << endl;
	treeFile.close();
}

void SeedTree::genTreeHelper(vector<NodeMeta> &parentNodes,
		vector<NodeMeta> &childrenNodes, unsigned int seedLen,
		unsigned int curIdx, ofstream &treeFile) {

//If max depth or if no more expensive seeds
	if (seedLen > maxSeedLen || parentNodes.empty())
		return;

	childrenNodes.clear();

	for (int i = 0; i < parentNodes.size(); i++) {

		TreeNode* parent = parentNodes[i].node;

		TreeNode* A = new TreeNode(parent->getSeed() + 'A');
		TreeNode* C = new TreeNode(parent->getSeed() + 'C');
		TreeNode* T = new TreeNode(parent->getSeed() + 'T');
		TreeNode* G = new TreeNode(parent->getSeed() + 'G');

		for (int j = 0; j < parent->getLocNum(); j++) {
			unsigned int loc = parent->getLoc(j);
			char base = ref.getRefBase(loc, seedLen);

			switch (base) {
			case 'A':
				A->addLoc(loc);
				break;
			case 'C':
				C->addLoc(loc);
				break;
			case 'T':
				T->addLoc(loc);
				break;
			case 'G':
				G->addLoc(loc);
				break;
			default:
				break;
			}
		}

		if (A->isEmpty()) {
			parent->setA(NULL);
			delete A;
		} else {
			parent->setA(A);
			//Write node to file give it parentIdx[i]

			treeFile << A->getSeed() << endl;

			for (int k = 0; k < A->getLocNum(); k++) {

				treeFile << A->getLoc(k) << " ";
			}
			treeFile << "-2";			// To denote end of location list
			treeFile << endl;
			treeFile << parentNodes[i].index << endl;

			if (A->getLocNum() >= expThreshold)
				childrenNodes.push_back(NodeMeta(A, curIdx));
			curIdx++;
		}

		if (C->isEmpty()) {
			parent->setC(NULL);
			delete C;
		} else {
			parent->setC(C);
			//Write node to file give it parentIdx[i]

			treeFile << C->getSeed() << endl;

			for (int k = 0; k < C->getLocNum(); k++) {

				treeFile << C->getLoc(k) << " ";
			}
			treeFile << "-2";			// To denote end of location list
			treeFile << endl;

			treeFile << parentNodes[i].index << endl;

			if (C->getLocNum() >= expThreshold)
				childrenNodes.push_back(NodeMeta(C, curIdx));
			curIdx++;
		}

		if (T->isEmpty()) {
			parent->setT(NULL);
			delete T;
		} else {
			parent->setT(T);
			//Write node to file give it parentIdx[i]

			treeFile << T->getSeed() << endl;

			for (int k = 0; k < T->getLocNum(); k++) {

				treeFile << T->getLoc(k) << " ";
			}
			treeFile << "-2";			// To denote end of location list
			treeFile << endl;

			treeFile << parentNodes[i].index << endl;

			if (T->getLocNum() >= expThreshold)
				childrenNodes.push_back(NodeMeta(T, curIdx));
			curIdx++;
		}

		if (G->isEmpty()) {
			parent->setG(NULL);
			delete G;
		} else {
			parent->setG(G);

			treeFile << G->getSeed() << endl;

			for (int k = 0; k < G->getLocNum(); k++) {

				treeFile << G->getLoc(k) << " ";
			}
			treeFile << "-2";			// To denote end of location list
			treeFile << endl;

			treeFile << parentNodes[i].index << endl;

			if (G->getLocNum() >= expThreshold)
				childrenNodes.push_back(NodeMeta(G, curIdx));
			curIdx++;
		}
	}

	genTreeHelper(childrenNodes, parentNodes, seedLen + 1, curIdx, treeFile);
}

void SeedTree::genTreeFake() {
	rootNodes.push_back(new TreeNode("AAAA", 20));
	rootNodes.push_back(new TreeNode("AAAC", 30));

	rootNodes[0]->setA(new TreeNode("AAAAA", 5));
	rootNodes[0]->setC(new TreeNode("AAAAC", 5));
	rootNodes[0]->setT(new TreeNode("AAAAT", 5));
	rootNodes[0]->setG(new TreeNode("AAAAG", 5));

	rootNodes[1]->setA(new TreeNode("AAACA", 15));
	rootNodes[1]->setC(new TreeNode("AAACC", 5));
	rootNodes[1]->setT(new TreeNode("AAACT", 5));
	rootNodes[1]->setG(new TreeNode("AAACG", 5));

}

void SeedTree::printRootNodes() {
	for (int i = 0; i < rootNodes.size(); i++) {
		cout << "rootNode: " << rootNodes[i]->getSeed() << endl;
	}
}

void SeedTree::printRootBranch(string rootSeed) {
	TreeNode treeNode(rootSeed);

//Locate the root
	vector<TreeNode*>::iterator tempIter = lower_bound(rootNodes.begin(),
			rootNodes.end(), &treeNode, compareNodePtr);

	if (tempIter == rootNodes.end()) {
		cout << "No expensive root seed found" << endl;
		return;
	}

	assert((*tempIter)->getSeed() == rootSeed);

	int index = tempIter - rootNodes.begin();

	cout << "LocNum: " << rootNodes[index]->getLocNum() << endl;

	for (int i = 0; i < rootEntries[index].size(); i++) {
		cout << rootEntries[index][i]->getSeed()
				<< rootEntries[index][i]->getLocNum() << endl;
		for (int j = 0; j < rootEntries[index][i]->getLocNum(); j++) {
			cout << rootEntries[index][i]->getLoc(j) << " ";
		}
		cout << endl;
	}

}

void SeedTree::printLeafNodes() {
	for (int i = 0; i < trueLeafNodes.size(); i++) {
		cout << "leafNode: " << trueLeafNodes[i]->getSeed() << " | ";
		cout << "size " << trueLeafNodes[i]->getLocNum() << endl;
		for (int j = 0; j < trueLeafNodes[i]->getLocNum(); j++)
			cout << trueLeafNodes[i]->getLoc(j) << " ";

		cout << endl;
	}
}

void SeedTree::prunePartialRepeats(int lowerBound) {
	if (trueLeafNodes.size() == 0) {
		cout << "Error, no leaf." << endl;
		return;
	}

	for (int i = trueLeafNodes.size() - 1; i >= 0; i--) {

		if (trueLeafNodes[i]->getSeedLength() <= lowerBound)
			continue;

		//cout << "Pruning seed " << leafNodes[i]->getSeed() << endl;
		for (int offset = 1;
				offset <= trueLeafNodes[i]->getSeed().length() - rootSeedLength;
				offset++) {

			string pruneSeed = trueLeafNodes[i]->getSeed().substr(offset);

			//cout << "Pruning Partial Seed " << pruneSeed << endl;

			removeLocListInBranch(pruneSeed, trueLeafNodes[i]->getLocList(),
					offset);
		}
	}
}

void SeedTree::pruneGenuineRepeats(int lowerBound) {
	if (trueLeafNodes.size() == 0) {
		cout << "Error, no leaf." << endl;
		return;
	}

	for (int i = trueLeafNodes.size() - 1; i >= 0; i--) {

		if (trueLeafNodes[i]->getSeedLength() <= lowerBound)
			continue;

		removeLocListInBranch(trueLeafNodes[i]->getSeed(),
				trueLeafNodes[i]->getLocList(), 0);
	}
}

//Does not work with pattern
void SeedTree::removeLocListInBranch(string endSeed,
		const vector<unsigned int>& locList, const int offset) {
	int iterLength = rootSeedLength;
	string base = endSeed.substr(0, iterLength);

	TreeNode treeNode(base);

//Locate the root
	vector<TreeNode*>::iterator tempIter = lower_bound(rootNodes.begin(),
			rootNodes.end(), &treeNode, compareNodePtr);

//No expensive root seed found, return
	if (tempIter == rootNodes.end() || (*tempIter)->getSeed() != base) {
		//cout << "Found seed " << (*tempIter)->getSeed() << endl;
		//cout << "No expensive root seed " << base << " found" << endl;

		return;
	}

	TreeNode* treePtr = *tempIter;

//Delete locations
	for (int i = 0; i < locList.size(); i++)
		treePtr->deleteLoc(locList[i] + offset);

	for (; iterLength < endSeed.length(); iterLength++) {

		switch (endSeed[iterLength]) {
		case 'A':
			treePtr = treePtr->getA();
			break;
		case 'C':
			treePtr = treePtr->getC();
			break;
		case 'T':
			treePtr = treePtr->getT();
			break;
		case 'G':
			treePtr = treePtr->getG();
			break;
		default:
			cerr << "Wrong base: " << endSeed[iterLength];
			exit(1);
		}

		//No children, return
		if (treePtr == NULL)
			return;
		//Else delete locations
		else {
			for (int i = 0; i < locList.size(); i++)
				treePtr->deleteLoc(locList[i] + offset);
		}
	}
}

void SeedTree::analyse(double percentage) {
	trueLeafNodes.clear();
	analyseBreadth(percentage, trueLeafNodes, true);
}

void SeedTree::evalHetSeed() {
	evalHetSeed(rootSeedLength + rootKmerLength);
}

void SeedTree::evalHetSeed(int maxLv) {
	hetSeedCost = 0;

	truncLeafNodes.clear();
	analyseBreadth(1, truncLeafNodes, false, maxLv);

	for (int i = 0; i < truncLeafNodes.size(); i++)
		hetSeedCost += (unsigned long int) truncLeafNodes[i]->getLocNum()
				* (unsigned long int) truncLeafNodes[i]->getLocNum();

	evalEntries();
}

void SeedTree::evalEntries() {
	rootEntries.clear();
	rootEntries.resize(rootNodes.size());

	TreeNode treeNode;

	for (int i = 0; i < truncLeafNodes.size(); i++) {
		string base = truncLeafNodes[i]->getSeed().substr(0, rootSeedLength);

		treeNode.setSeed(base);

		//Locate the root
		vector<TreeNode*>::iterator tempIter = lower_bound(rootNodes.begin(),
				rootNodes.end(), &treeNode, compareNodePtr);

		//No expensive root seed found, return
		assert(tempIter != rootNodes.end() || (*tempIter)->getSeed() == base);

		rootEntries[tempIter - rootNodes.begin()].push_back(truncLeafNodes[i]);
	}
}

void SeedTree::checkRoots() {
	expensiveSeedCost = 0;
	for (vector<TreeNode*>::iterator iter = rootNodes.begin();
			iter != rootNodes.end(); iter++) {
		if ((*iter)->getLocNum() < expThreshold) {
			cheapSeedCost += (*iter)->getLocNum() * (*iter)->getLocNum();
			delete (*iter);
			iter = rootNodes.erase(iter);
			iter--;
		} else
			expensiveSeedCost += (*iter)->getLocNum() * (*iter)->getLocNum();
	}
}

void SeedTree::analyseBreadth(double percentage, vector<TreeNode*> &leafNodes,
		bool analysis, int maxDepth) {
	if (analysis) {
		expNum.clear();
		seedLvs.clear();
		evenSplits.clear();
	}

	vector<TreeNode*> parentNodes;
	vector<TreeNode*> childrenNodes;

	expNum.clear();

	parentNodes = rootNodes;

	analyseBreadthHelper(parentNodes, childrenNodes, percentage,
			maxDepth - rootSeedLength, leafNodes, analysis);
}

void SeedTree::analyseBreadthHelper(vector<TreeNode*> &parentNodes,
		vector<TreeNode*> &childrenNodes, double percentage, int availableDepth,
		vector<TreeNode*> &leafNodes, bool analysis) {
//Just to populate the vector
	repeatCoverages.push_back(0);

//Return if we reach the bottom
	if (parentNodes.size() == 0)
		return;

//Every parentNode become leaf Nodes at bottom
	if (availableDepth == 0) {
		for (int i = 0; i < parentNodes.size(); i++)
			leafNodes.push_back(parentNodes[i]);
		return;
	}

	childrenNodes.clear();

//Analysis data
	map<unsigned int, unsigned int> seedLv;
	unsigned int evenSplit = 0;
	TreeNode* tempNode;

	for (int i = 0; i < parentNodes.size(); i++) {
		unsigned int maxLoc = 0;

		for (int j = 0; j < 4; j++) {

			switch (j) {
			case 0:
				tempNode = parentNodes[i]->getA();
				break;
			case 1:
				tempNode = parentNodes[i]->getC();
				break;
			case 2:
				tempNode = parentNodes[i]->getT();
				break;
			case 3:
				tempNode = parentNodes[i]->getG();
				break;
			}

			if (tempNode != NULL) {

				if (analysis && tempNode->getLocNum() > maxLoc)
					maxLoc = tempNode->getLocNum();

				if (tempNode->getLocNum() >= expThreshold)
					childrenNodes.push_back(tempNode);
				else {
					leafNodes.push_back(tempNode);

					if (!tempNode->isLeaf()) {
						//Delete this node after pruning
						if (!analysis)
							tempNode->deleteChildren();
					}
				}

			}

		}

		if (analysis) {
			//Get ExpensivenessDist
			unsigned int log_loc = 31
					- __builtin_clz(parentNodes[i]->getLocNum());

			//Dump seedLv
			map<unsigned int, unsigned int>::iterator iter = seedLv.find(
					log_loc);
			if (iter == seedLv.end())
				seedLv[log_loc] = 1;
			else
				iter->second++;

			//Get split Info
			if (maxLoc <= percentage * parentNodes[i]->getLocNum())
				evenSplit++;
		}
	}

	if (analysis) {
		seedLvs.push_back(seedLv);
		expNum.push_back(parentNodes.size());
		evenSplits.push_back(evenSplit);
	}

	analyseBreadthHelper(childrenNodes, parentNodes, percentage,
			availableDepth - 1, leafNodes, analysis);
}

void SeedTree::evalRepeatCoverage() {
	if (trueLeafNodes.size() == 0) {
		cout << "Error, no leaf." << endl;
		return;
	}

	repeatCoverages.clear();

	for (int i = 0; i < trueLeafNodes.size(); i++) {
		repeatCoverages[trueLeafNodes[i]->getSeed().length() - rootSeedLength] +=
				trueLeafNodes[i]->getLocNum()
						* trueLeafNodes[i]->getSeed().length();
	}
}

void SeedTree::printAnalysisData() {
	printExpSeedNum();
	printSeedCompo();
	printSplit();
}

void SeedTree::printEvaluationData(int entryThreshold) {
	cout << "====Hash Table====" << endl;
	cout << "Cheap Seed Mapping Cost: " << cheapSeedCost << endl;
	cout << "Expensive Seed Mapping Cost: " << expensiveSeedCost << endl;
	cout << "Heterogeneous Seed Mapping Cost: " << hetSeedCost << endl;
	cout << "Heterogeneous Seed Storage Cost: " << truncLeafNodes.size()
			<< endl;
	cout << "Original Hash Table Storage Cost: " << (1 << (2 * rootKmerLength))
			<< endl;

	printEntryCost();
	printEntryCostCompo();
	printEntrySizeCompo();
//	printEntries(entryThreshold);
}

void SeedTree::printEntryCost() {
	unsigned int entryCost = 0;

	for (int i = 0; i < rootNodes.size(); i++)
		entryCost += rootNodes[i]->getLocNum() * rootEntries[i].size();

	cout << "Entry Total Cost: " << entryCost << endl;
}

void SeedTree::printEntryCostCompo() {
	map<unsigned int, unsigned int> entryLv;

	map<unsigned int, unsigned int>::iterator iter;

	cout << "Printing Entry Cost Composition: " << endl;

	for (int i = 0; i < rootEntries.size(); i++) {
		for (int j = 0; j < rootEntries[i].size(); j++) {
			unsigned int log_loc = 31
					- __builtin_clz(rootEntries[i][j]->getLocNum());

			//Dump seedLv
			iter = entryLv.find(log_loc);
			if (iter == entryLv.end())
				entryLv[log_loc] = 1;
			else
				iter->second++;
		}
	}

	iter = entryLv.begin();
	int MinLv = iter->first;

	iter = entryLv.end();
	iter--;
	int MaxLv = iter->first;

	for (int i = MinLv; i <= MaxLv; i++) {
		cout << i << "\t| ";
	}
	cout << endl;

	for (int i = MinLv; i <= MaxLv; i++) {
		if (entryLv.find(i) == entryLv.end())
			cout << "0\t| ";
		else
			cout << entryLv[i] << "\t| ";
	}
	cout << endl;

}

void SeedTree::printEntrySizeCompo() {
	map<unsigned int, unsigned int> entryLv;

	map<unsigned int, unsigned int>::iterator iter;

	unsigned int totalEntries = 0;
	unsigned int maxEntries = 0;

	cout << "Printing Entry Size Composition: " << endl;

	for (int i = 0; i < rootEntries.size(); i++) {
		if (rootEntries[i].size() > maxEntries)
			maxEntries = rootEntries[i].size();

		unsigned int log_loc = 31 - __builtin_clz(rootEntries[i].size());
		totalEntries += rootEntries[i].size();

		//Dump seedLv
		iter = entryLv.find(log_loc);
		if (iter == entryLv.end())
			entryLv[log_loc] = 1;
		else
			iter->second++;
	}

	iter = entryLv.begin();
	int MinLv = iter->first;

	iter = entryLv.end();
	iter--;
	int MaxLv = iter->first;

	for (int i = MinLv; i <= MaxLv; i++) {
		cout << i << "\t| ";
	}
	cout << endl;

	for (int i = MinLv; i <= MaxLv; i++) {
		if (entryLv.find(i) == entryLv.end())
			cout << "0\t| ";
		else
			cout << entryLv[i] << "\t| ";
	}
	cout << endl;

	cout << "The total number of entries is: " << totalEntries << endl;
	cout << "The most expensive seed has " << maxEntries << " entries" << endl;

}

bool leafNodeCompare(TreeNode * lfs, TreeNode * rhs) {
	return lfs->getSeed() < rhs->getSeed();
}

void SeedTree::printEntries(int entryThreshold) {
	cout << "Printing Entries of the Root Seeds" << endl;

	for (int i = 0; i < rootNodes.size(); i++) {
		if (rootEntries[i].size() < entryThreshold)
			continue;

		cout << "Root seed: " << rootNodes[i]->getSeed() << " | "
				<< rootNodes[i]->getLocNum() << endl;

		for (int j = 0; j < rootEntries[i].size(); j++) {
//			cout << rootEntries[i][j]->getSeed() << endl;
			if (rootEntries[i][j] == NULL) {
				cerr << "NULL Pointer of " << rootNodes[i]->getSeed() << ": "
						<< j << endl;
				exit(1);
			}
		}

		sort(rootEntries[i].begin(), rootEntries[i].end(), leafNodeCompare);

		for (int j = 0; j < rootEntries[i].size(); j++) {
			cout << rootEntries[i][j]->getSeed() << " | "
					<< rootEntries[i][j]->getLocNum() << endl;
		}
	}
}

void SeedTree::printExpSeedNum() {
	cout << "Printing Expensive Seed Count" << endl;
	cout << "SeedLenth \t| Seed Count" << endl;

	for (int i = 0; i < expNum.size(); i++)
		cout << rootSeedLength + i << "\t| " << expNum[i] << endl;
}

void SeedTree::printSeedCompo() {
	int MinLv, MaxLv;
	map<unsigned int, unsigned int>::iterator iter;

	iter = seedLvs[0].begin();
	MinLv = iter->first;

	iter = seedLvs[0].end();
	iter--;
	MaxLv = iter->first;

	cout << "Printing Expensive Seed Composition" << endl;
	cout << "SeedLen\t| ";
	for (int j = MinLv; j <= MaxLv; j++) {
		cout << j << "\t| ";
	}
	cout << endl;

	for (int i = 0; i < seedLvs.size(); i++) {
		cout << rootSeedLength + i << "\t| ";
		assert(MinLv <= seedLvs[i].begin()->first);
		for (int j = MinLv; j <= MaxLv; j++) {
			if (seedLvs[i].find(j) == seedLvs[i].end())
				cout << "0\t| ";
			else
				cout << seedLvs[i][j] << "\t| ";
		}
		cout << endl;
	}
}

void SeedTree::printSplit() {
	cout << "Printing Even splits" << endl;
	cout << "SeedLen\t| EvenSplits" << endl;

	for (int i = 0; i < evenSplits.size(); i++)
		cout << rootSeedLength + i << "\t| " << evenSplits[i] << endl;
}

void SeedTree::printRepeatCoverage() {
	cout << "Printing Repeat Coverage" << endl;
	cout << "SeedLen\t| Coverage" << endl;

	for (int i = 0; i < evenSplits.size(); i++)
		cout << rootSeedLength + i << "\t| " << repeatCoverages[i] << endl;
}

void SeedTree::loadTree(string treeFile) {

	ifstream readTree;
	readTree.open(treeFile.c_str());
	string kmer;
	unsigned int loc;
	int parentId;
	vector<TreeNode*> tempNodes;
	char base;

	while (readTree >> kmer) {

		if (kmer == "end")			// write end to denote nodes have ended
			break;
		TreeNode* tempNode = new TreeNode(kmer);

		loc = 0;
		while (loc != -2) {
			readTree >> loc;
			if (loc != -2)
				tempNode->addLoc(loc);
		}
		readTree >> parentId;

		if (parentId == -1) {
			rootNodes.push_back(tempNode);
		}

		else {
			base = kmer[kmer.size() - 1];	// Get last character of kmer

			switch (base) {
			case 'A':
				tempNodes[parentId]->setA(tempNode);
				break;
			case 'C':
				tempNodes[parentId]->setC(tempNode);
				break;
			case 'T':
				tempNodes[parentId]->setT(tempNode);
				break;
			case 'G':
				tempNodes[parentId]->setG(tempNode);
				break;
			default:
				break;
			}

		}

		tempNodes.push_back(tempNode);

	}
	readTree >> expThreshold;
	readTree >> maxSeedLen;
	readTree >> cheapSeedCost;
	readTree >> expensiveSeedCost;
	readTree >> rootSeedLength;
	readTree >> rootKmerLength;
}

void SeedTree::genXml(string directoryPath) {

	string nodeFile;
	string kmer;
	string edgeFile;

	for (int i = 0; i < rootNodes.size(); i++) {

		kmer = rootNodes[i]->getSeed();

		nodeFile = directoryPath + kmer + ".gexf";
		edgeFile = directoryPath + "e" + kmer + ".gexf";

		nfile.open(nodeFile.c_str());
		efile.open(edgeFile.c_str());

		writeHeader();
		startEdge(); // to start edge file

		dumpXml(rootNodes[i], "super", ""); // if string is "" -- edge label b/w super and root

		efile.close();
		if (appendFile(edgeFile) != 0)
			cout << "Error";

		nfile.close();

	}

	system("cd gexf/");
	system("find . -name \"e*.gexf\" | xargs rm");

}

void SeedTree::dumpXml(TreeNode * root, string parent, string base) {

	ostringstream converter;
	int log_loc;

	if (root == NULL)
		return;

	string kmer = root->getSeed();

	converter << root->getLocNum();

	string locNum = converter.str();

	log_loc = 31 - __builtin_clz(root->getLocNum());

	converter.str("");

	converter << log_loc;

	string loc_lg = converter.str();

	addNode(kmer, locNum, loc_lg);

	if (parent != "super")
		addEdge(kmer, parent, base);

	dumpXml(root->getA(), kmer, "A");

	dumpXml(root->getC(), kmer, "C");

	dumpXml(root->getT(), kmer, "T");

	dumpXml(root->getG(), kmer, "G");

}

void SeedTree::addNode(string label, string loc, string loc_log) {
	string data;

	data = "\n\t\t\t<node id=\"";
	data += label;					// since now id=label

	data += "\" label=\"";
	data += label;

	data += "\">\n\t\t\t<attvalues>\n\t\t\t\t<attvalue for=\"0\" value=\"";
	data += loc;

	data += "\"/>\n\t\t\t\t<attvalue for=\"1\" value=\"";
	data += loc_log;

	data += "\"/> \n\t\t\t</attvalues>\n\t\t</node>";

	nfile << data;
}

void SeedTree::addEdge(string target, string source, string base) {

	string edata;
	edata = "\n\t\t\t<edge id=\"";
	edata += target;

	edata += "\" label=\"";
	edata += base;

	edata += "\" source=\"";
	edata += source;

	edata += "\" target=\"";
	edata += target;
	edata += "\"/>";

	efile << edata;
}

void SeedTree::writeHeader() {

	nfile
			<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<gexf xmlns=\"http://www.gexf.net/1.1draft\" version=\"1.1\">\n\t<meta>\n\t\t<creator>Hitesh Arora n Xin Hongyi</creator>\n\t\t<description>Superfast</description>\n\t</meta>\n\t<graph mode=\"static\" defaultedgetype=\"directed\">\n\t\t<attributes class=\"node\">\n\t\t\t<attribute id=\"0\" title=\"KmerSize\" type=\"integer\">\n\t\t\t\t<default>0</default>\n\t\t\t</attribute>\n\t\t\t<attribute id=\"1\" title=\"KmerSizeLog\" type=\"integer\">\n\t\t\t\t<default>0</default>\n\t\t\t</attribute>\n\t\t</attributes>\n\t\t<nodes>";

}

void SeedTree::startEdge() {

	efile << "\n\t\t</nodes> \n\t\t<edges>";
}

int SeedTree::appendFile(string edgeFile) {

	ifstream edfile;
	edfile.open(edgeFile.c_str());

	string line;

	if (edfile.is_open()) {
		while (!(edfile.eof())) {
			getline(edfile, line);
			nfile << line;
			nfile << "\n";
		}

		edfile.close();
		nfile << "\n\t\t</edges>\n\t</graph>\n</gexf>";
	}
// writing end

	else
		cout << "Error in appending file" << endl;

	return 0;
}

