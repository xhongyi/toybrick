/*
 * vector_filterMain.c
 *
 *  Created on: Nov 12, 2013
 *      Author: hxin
 */

//#ifndef BOOST_PP_IS_ITERATING
//#include "print.h"
#include <cstdlib>
#include "emulate_vector.h"
#include <string.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
	string read;
	string ref;

	if (argc != 3) {
		cout << "Usage: $>bin error min_match" << endl;
		exit(1);
	}

	int error = atoi(argv[1]);
	int min_match = atoi(argv[2]);

	vector_filter emulator(error, min_match);

	size_t lineLength;
	size_t length;
	char* tempstr = NULL;

	long long unsigned int passNum = 0;
	long long unsigned int totalNum = 0;

	do {
		cin >> read;
		if (read == "end_of_file")
			break;
		cin >> ref;

		emulator.set_read(read);
		emulator.set_ref(ref);	
		if (emulator.check_match() ) {
			cout << read << endl;
			cout << ref << endl;
			passNum++;
		}

		totalNum++;
	} while (1);

	cout << "end_of_file\n";
	cerr << "passNum:\t" << passNum << endl;
	cerr << "totalNum:\t" << totalNum << endl;

	return 0;

}

//#endif
