#include <string>

using namespace std;

class vector_filter {

public:
	vector_filter();	
	vector_filter(int error, int min_matching);
	~vector_filter();

	void set_error(int error);
	void set_min_matching(int min_matching);

	void set_length(length);
	void set_read(string read);
	void set_ref(string ref);
	
	bool check_match();
private:
	//Initial data
	bool** bit_vector;
	int error;
	int min_matching;

	//Internal data
	int length;
	string read;
	string ref;
};
