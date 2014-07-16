/*
 * vector_filterMain.c
 *
 *  Created on: Nov 12, 2013
 *      Author: hxin
 */

//#ifndef BOOST_PP_IS_ITERATING
//#include "print.h"
#include <string>
#include <sys/times.h>
#include <unistd.h>
#include <stdint.h>
#include "vector_filter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <emmintrin.h>

#define BATCH_RUN 1000000 
#define CACHE_LINE_SIZE 64 // bytes per line

using namespace std;

#include "mask.h"

//char read[128];
//char ref[128];

char init_all_NULL[128] = "";

char read_t[128] __aligned__;// = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
char ref_t[128] __aligned__;// = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

int main(int argc, char* argv[]) {
	
	string *read_strs = new string [BATCH_RUN];
	string *ref_strs = new string [BATCH_RUN];
	bool *valid_buff = new bool [BATCH_RUN];

	if (argc != 3) {
		printf("Usage: $>bin error\n");
		exit(1);
	}

	/*
	 * capture the CLI inputs 
	 */
	const int error = atoi(argv[1]);
	const int PREFETCH_HEAD_START = atoi(argv[2]);

	// declare a helper array for testing
	int* random_indices; 
	

	size_t lineLength;	
	size_t length;
	char* tempstr = NULL;

	long long unsigned int passNum = 0;
	long long unsigned int totalNum = 0;

	long long read_size;
	long long read_idx;
	bool stop = false;

	tms start_time;
	tms end_time;
	tms elp_time;

	elp_time.tms_stime = 0;
	elp_time.tms_utime = 0;
	elp_time.tms_cstime = 0;
	elp_time.tms_cutime = 0;

	do {
		//clear past result
//		strncpy(read, init_all_NULL, 128);
//		strncpy(ref, init_all_NULL, 128);
		
		for (read_size = 0; read_size < BATCH_RUN; read_size++) {
			
			//get read
			getline(&tempstr, &lineLength, stdin);
			length = strlen(tempstr);
			//Get rid of the new line character
			tempstr[length - 1] = '\0';
			
			if (strcmp(tempstr, "end_of_file\0") == 0) {
				stop = true;
				break;
			}
			read_strs[read_size].assign(tempstr);

			//get ref
			getline(&tempstr, &lineLength, stdin);
			length = strlen(tempstr);
			//Get rid of the new line character
			tempstr[length - 1] = '\0';
			ref_strs[read_size].assign(tempstr);
			valid_buff[read_size] = false;
		}

		/*
		 * fill an array with random indices in the read and ref array to access
		 * designed to simulate random access
		 */
		
		random_indices = new int[read_size];
		for(int i = 0; i < read_size; i++){
		  random_indices[i] = rand() % read_size; 
		}

		times(&start_time);

		/*
		 * get the prefetches that are before the head start
		 */
		int i;
		for (i = 0; i < PREFETCH_HEAD_START; i++){

		  /*
		   * _mm_prefetch only pulls in enough data to fill one cache line
		   * so we must make two requests for each read and ref pair
		   */
  		  _mm_prefetch(read_strs + random_indices[i], _MM_HINT_NTA);
		  _mm_prefetch(read_strs[random_indices[i]].c_str() + CACHE_LINE_SIZE, _MM_HINT_NTA);

		  _mm_prefetch(ref_strs + random_indices[i], _MM_HINT_NTA);
		  _mm_prefetch(ref_strs[random_indices[i]].c_str() + CACHE_LINE_SIZE, _MM_HINT_NTA);

		}

		/*
		 * begin the main loop (stops PREFETCH_HEAD_START from the end)
		 */
		for (i = 0; i < read_size - PREFETCH_HEAD_START; i++) {
			
		  // make the next prefetch request
		  const int next_prefetch = random_indices[i + PREFETCH_HEAD_START];

		  _mm_prefetch(read_strs + next_prefetch, _MM_HINT_NTA);
                  _mm_prefetch(read_strs[next_prefetch].c_str() + CACHE_LINE_SIZE, _MM_HINT_NTA);

                  _mm_prefetch(ref_strs + next_prefetch, _MM_HINT_NTA);
                  _mm_prefetch(ref_strs[next_prefetch].c_str() + CACHE_LINE_SIZE, _MM_HINT_NTA);
		  
		  // perform bitvector computations on the 
			strncpy(read_t, init_all_NULL, 128);
			strncpy(ref_t, init_all_NULL, 128);

			length = read_strs[random_indices[i]].length();

			if (length > 128)
				length = 128;
			strncpy(read_t, read_strs[random_indices[i]].c_str(), length);

			length = ref_strs[random_indices[i]].length();
			//Get rid of the new line character
			if (length > 128)
				length = 128;
			strncpy(ref_t, ref_strs[random_indices[i]].c_str(), length);

			if (bit_vec_filter_sse1(read_t, ref_t, length, error))
				valid_buff[random_indices[i]] = true;
		}

		/*
		 * perform the last few bitvector comutations (prefetching has reached the end)
		 * read_idx is not inititalized because we are continuing where we left off
		 */
		for (; i < read_size; i++) {
		
		  // perform bitvector computations on the 
			strncpy(read_t, init_all_NULL, 128);
			strncpy(ref_t, init_all_NULL, 128);

			length = read_strs[random_indices[i]].length();

			if (length > 128)
				length = 128;
			strncpy(read_t, read_strs[random_indices[i]].c_str(), length);

			length = ref_strs[random_indices[i]].length();
			//Get rid of the new line character
			if (length > 128)
				length = 128;
			strncpy(ref_t, ref_strs[random_indices[i]].c_str(), length);

			if (bit_vec_filter_sse1(read_t, ref_t, length, error))
				valid_buff[random_indices[i]] = true;
		}


		times(&end_time);

		for (read_idx = 0; read_idx < read_size; read_idx++) {

			if (valid_buff[read_idx]) {
			  //    fprintf(stderr, "%.*s\n", 128, read_strs[read_idx].c_str() );
			  //	fprintf(stderr, "%.*s\n", 128, ref_strs[read_idx].c_str() );
				passNum++;
			}
			totalNum++;
		}

		
		elp_time.tms_stime += end_time.tms_stime - start_time.tms_stime;
		elp_time.tms_utime += end_time.tms_utime - start_time.tms_utime;

		if (stop)
			break;

	} while (1);

	fprintf(stderr, "end_of_file\n");
	printf("passNum:\t%lld\n", passNum);
	printf("totalNum:\t%lld\n", totalNum);
	printf("total_time: %f\n", (double) elp_time.tms_utime / sysconf(_SC_CLK_TCK) ); 

	delete [] read_strs;
	delete [] ref_strs;

	return 0;

}

//#endif
