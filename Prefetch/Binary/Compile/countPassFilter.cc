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
#include <fstream>
#include <iostream>

#define BATCH_RUN 1000000 
#define RANDOM_NUM_FILE_1 "random_num.inp"
#define RANDOM_NUM_FILE_2 "prefetch_random_num.inp"
#define BLOCK_SIZE 1000


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

  /*
    if (argc != 2) {
    printf("Usage: $>bin error\n");
    exit(1);
    }*/

  // TESTING FOR PREFETCH SIZE
  const int PREFETCH_HEAD_START = atoi(argv[2]);

  int error = atoi(argv[1]);

  size_t lineLength;	
  size_t length;
  char* tempstr = NULL;

  long long unsigned int passNum = 0;
  long long unsigned int totalNum = 0;

  long long read_size;
  long long read_idx;
  int As = 0;
  bool stop = false;

  tms start_time;
  tms end_time;
  tms elp_time;
  double time_elapsed = 0.0;

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

    // create an array to store the random indices
    int compute_indices[BLOCK_SIZE];
    int prefetch_indices[BLOCK_SIZE];

    ifstream randFile_1(RANDOM_NUM_FILE_1);
    ifstream randFile_2(RANDOM_NUM_FILE_2);

    if( !randFile_1.is_open() || !randFile_2.is_open()){
      cout << "failed to open the random number file\n";
      cout << "check: " << RANDOM_NUM_FILE_1 << " and " 
	   << RANDOM_NUM_FILE_2 << endl;
    }

    // throw away the first few elements in the prefetch random num file
    int garbage;
    for(int i = 0; i < PREFETCH_HEAD_START; i++){
      randFile_2 >> garbage;
    }

    for (read_idx = 0; read_idx < read_size - BLOCK_SIZE; read_idx+=BLOCK_SIZE) {


      // fill the compute index array
      for(int i = 0; i < BLOCK_SIZE; i++){
       	randFile_1 >> compute_indices[i];
      }

      bool filled = false;
      if(read_idx + PREFETCH_HEAD_START < read_size){
	filled = true;
	// fill the prefetch index array
	for(int i = 0; i < BLOCK_SIZE; i++){
	  randFile_2 >> prefetch_indices[i];
	}
      }

      elp_time.tms_stime = 0;
      elp_time.tms_utime = 0;
      elp_time.tms_cstime = 0;
      elp_time.tms_cutime = 0;
		  
      times(&start_time);
      


      /*
       * begin the main loop
       */
      int i;

      times(&start_time);
      for(i = 0; i < BLOCK_SIZE; i++){

	if(filled){
	  _mm_prefetch(read_strs + prefetch_indices[i], _MM_HINT_T0);
	  _mm_prefetch(read_strs[prefetch_indices[i]].c_str() + 64, _MM_HINT_T0);
		    
	  _mm_prefetch(ref_strs + prefetch_indices[i], _MM_HINT_T0);
	  _mm_prefetch(ref_strs[prefetch_indices[i]].c_str() + 64, _MM_HINT_T0);
	}

	strncpy(read_t, init_all_NULL, 128);
	strncpy(ref_t, init_all_NULL, 128);
		    
	length = read_strs[compute_indices[i]].length();
		    
	if (length > 128)
	  length = 128;
	strncpy(read_t, read_strs[compute_indices[i]].c_str(), length);
		    
	length = ref_strs[compute_indices[i]].length();
	//Get rid of the new line character
	if (length > 128)
	  length = 128;
	strncpy(ref_t, ref_strs[compute_indices[i]].c_str(), length);

	if (bit_vec_filter_sse1(read_t, ref_t, length, error))// this is where bit vector convert occurs
	  valid_buff[compute_indices[i]] = true;
      }

      /*
       * end the timer and record how long the looping took
       */
      times(&end_time);

      elp_time.tms_stime += end_time.tms_stime - start_time.tms_stime;
      elp_time.tms_utime += end_time.tms_utime - start_time.tms_utime;

      time_elapsed += (double) elp_time.tms_utime / sysconf(_SC_CLK_TCK);
    }



    for (read_idx = 0; read_idx < read_size; read_idx++) {

      if (valid_buff[read_idx]) {
	//fprintf(stderr, "%.*s\n", 128, read_strs[read_idx].c_str() );
	//fprintf(stderr, "%.*s\n", 128, ref_strs[read_idx].c_str() );
	passNum++;
      }
      totalNum++;
    }

    if (stop)
      break;

  } while (1);

  fprintf(stderr, "end_of_file\n");
  printf("passNum:\t%lld\n", passNum);
  printf("totalNum:\t%lld\n", totalNum);
  //	printf("total_time: %f\n", (double) elp_time.tms_utime / sysconf(_SC_CLK_TCK) ); 
  printf("total_time: %f\n", time_elapsed ); 
	

  delete [] read_strs;
  delete [] ref_strs;

  return 0;

}

//#endif
