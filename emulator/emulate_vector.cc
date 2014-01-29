#include "emulate_vector.h"
#include <cassert>
#include <iostream>

vector_filter::vector_filter() {
	bit_vector = NULL;
	length = 0;
	set_error(0);
	min_matching = 0;
};

vector_filter::vector_filter(int error, int min_matching) {
	bit_vector = NULL;
	length = 0;
	set_error(error);
	this->min_matching = min_matching;
}

vector_filter::~vector_filter() {
	if (bit_vector != NULL) {
		assert(this->error == 0);
		for (int i = 0; i < (1 + 2 * this->error); i++)
			delete [] bit_vector[i];

		delete [] bit_vector;
		bit_vector = NULL;
	} 
}

void vector_filter::set_error(int error) {
	if (this->error == error)
		return;

	if (bit_vector != NULL) {
		assert(this->error == 0);
		for (int i = 0; i < (1 + 2 * this->error); i++)
			delete [] bit_vector[i];

		delete [] bit_vector;
		bit_vector = NULL;
	}

	this->error = error;

	cout << "error: " << error << endl;

	bit_vector = new bool*[1 + 2 * error];
	bit_vector[0] = NULL;

	for (int i = 0; i < (1 + 2 * error); i++) {
		cout << "i: " << i << endl;
		if (length == 0)
			bit_vector[i] = NULL;
		else
			bit_vector[i] = new bool[length]();
	}
}

void vector_filter::set_length(int length) {
	cout << "input length: " << length << endl;
	
	if (length == this->length)
		return;

	cout << length << " this->length: " << this->length << endl;
	
	this->length = length;
	
	for (int i = 1; i <= (1 + 2 * error); i++) {
		if (bit_vector[i] != NULL) {
			delete [] bit_vector[i];
			bit_vector[i] = NULL;
		}
	}

	if (length == 0)
		return;

	for (int i = 0; i < (1 + 2 * error); i++)
		bit_vector[i] = new bool[length]();
}

void vector_filter::set_min_matching (int min_matching) {
	this->min_matching = min_matching;
}

void vector_filter::set_read (string read) {
	this->read = read;
}

void vector_filter::set_ref (string ref) {
	this->ref = ref;
}

bool vector_filter::check_match() {
	if (read.length() < ref.length() )
		set_length(read.length() );
	else
		set_length(ref.length() );

	cout << read << endl;
	cout << ref << endl;

	for (int j = 0; j < length; j++) {
		bit_vector[error][j] = (read[j] == ref[j]);
	}

	for (int j = 0; j < length; j++) {
			if (bit_vector[error][j])
				cout << '1';
			else
				cout << '0';
		}
	cout << endl;


	cout << "still alive 0" << endl;

	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			bit_vector[error-i][j] = (j < i || read[j - i] == ref[j]);
			bit_vector[error+i][j] = (j < i || read[j] == ref[j - i]);
		}
	}
	
	cout << "still alive 0" << endl;

	for (int i = 0; i < (1 + 2 * error); i++)
		flip_bits(i);

	return false;
}

void vector_filter::flip_bits(int index) {
	int head = 0;
	int tail = 0;
	int last = 0;
	bool flipping = false;

	while (head < length) {
		if (bit_vector[index][head]) {
			if (flipping == false) {
				flipping = true;
				tail = head;
				last = head;
			} else {
				last = head;
			}
		}
		else {
			if (flipping == true) {
				if (head - last >= min_matching) {
					while (tail <= last) {
						bit_vector[index][tail] = true;
						tail++;
					}
					flipping = false;
					tail = head;
					last = head;
				}
			}
			else {
				tail = head;
				last = head;
			}
		}

		head++;
			
	}

	if (flipping == true) {
		while (tail <= last) {
			bit_vector[index][tail] = true;
			tail++;
		}
	}

	for (int i = 0; i < length; i++) {
		if (bit_vector[index][i])
			cout << '1';
		else
			cout << '0';
	}
	cout << endl;
}
