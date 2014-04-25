#include "emulate_vector.h"
#include <cassert>
#include <iostream>

vector_filter::vector_filter() {
	bit_vector = NULL;
	result_vector = NULL;
	error = -1;
	length = -1;
	min_matching = -1;
};

vector_filter::vector_filter(int error, int min_matching) {
	bit_vector = NULL;
	result_vector = NULL;
	length = -1;
	this->error = -1;
	this->min_matching = min_matching;
	set_error(error);
}

vector_filter::~vector_filter() {
	if (bit_vector != NULL) {
		for (int i = 0; i < (1 + 2 * error); i++)
			delete [] bit_vector[i];

		delete [] bit_vector;
		bit_vector = NULL;
	}
	if (result_vector != NULL) {
		delete [] result_vector;
		result_vector = NULL;
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

//	cout << "error: " << error << endl;

	bit_vector = new bool*[1 + 2 * error];
	bit_vector[0] = NULL;

	for (int i = 0; i < (1 + 2 * error); i++) {
		if (length <= 0)
			bit_vector[i] = NULL;
		else
			bit_vector[i] = new bool[length]();
	}
}

void vector_filter::set_length(int length) {
	//cout << "input length: " << length << endl;
	
	if (length == this->length)
		return;

	//cout << " this->length: " << this->length << endl;
	
	this->length = length;

	for (int i = 0; i < (1 + 2 * error); i++) {
		if (bit_vector[i] != NULL) {
			delete [] bit_vector[i];
			bit_vector[i] = NULL;
		}
		if (result_vector != NULL) {
			delete [] result_vector;
			result_vector = NULL;
		}
	}

	if (length == 0)
		return;

	for (int i = 0; i < (1 + 2 * error); i++)
		bit_vector[i] = new bool[length]();
	result_vector = new bool[length]();
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

	//cout << read << endl;
	//cout << ref << endl;

	for (int j = 0; j < length; j++) {
		bit_vector[error][j] = !(read[j] == ref[j]);
	}

	for (int i = 1; i <= error; i++) {
		for (int j = 0; j < length; j++) {
			//cout << "error - 1: " << error - i << endl; 
			bit_vector[error-i][j] = !(j < i || read[j - i] == ref[j]);
			bit_vector[error+i][j] = !(j < i || read[j] == ref[j - i]);
		}
	}
	
	for (int i = 0; i < (1 + 2 * error); i++)
		flip_bits(i);

	for (int j = 0; j < length; j++) {
		result_vector[j] = true;
		for (int i = 0; i < 1 + 2 * error; i++) {
			result_vector[j] &= bit_vector[i][j];
		}
	}

	for (int j = 0; j < length; j++) {
		if (result_vector[j])
			cout << '1';
		else
			cout << '0';
	}
	cout << endl;

	int errorCounter = 0;
	int head = 0;
	int tail = 0;
	bool in_error = false;

	while (head < length) {
		if (result_vector[head]) {
			if (!in_error) {
				in_error = true;
				tail = head;	
			}	
		}
		else {
			if (in_error) {
				in_error = false;
				if (min_matching == 0)
					errorCounter += head - tail;
				else
					errorCounter += 1 + (head - tail + min_matching	- 2) / min_matching;
				if (errorCounter > error)
					return false;
			}
		}

		head++;	
	}
	if (in_error) {
		if (min_matching == 0)
			errorCounter += head - tail;
		else
			errorCounter += 1 + (head + min_matching - 2 - tail) / min_matching;
	}

	//cout << "errorCounter " << errorCounter << endl;

	return (errorCounter <= error);
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
