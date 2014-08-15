#include <Python.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define LENGTH 0
#define INDEX 1
#define OUT_LINE_LEN 8

int* validLocs;

/**
 *
 * FUNCTION: seeds2string
 * converts a list of seeds to a human readable string
 *
 */
char* seeds2string(int** seedsList, int seedsListLen){

  char* output;
  output = malloc((seedsListLen + 1) * OUT_LINE_LEN * sizeof(char));

  // add the header
  strcpy(output, "STR\tLEN\n");

  for(int i = 0; i < seedsListLen; i++){
    char helper1[4], helper2[4];
    sprintf(helper1, "%d\t", seedsList[i][INDEX]);
    sprintf(helper2, "%d\n", seedsList[i][LENGTH]);

    strcat(output, helper1);
    strcat(output, helper2);
  }

  return output;
}

void copy_seeds_list(int** currSeedsList, int** optimalSeedsList, int seedsListLen){

  for(int i = 0; i < seedsListLen; i++){
    optimalSeedsList[i][LENGTH] = currSeedsList[i][LENGTH];
    optimalSeedsList[i][INDEX] = currSeedsList[i][INDEX];
  }

}



/**
 *
 * FUNCTION: num_loc_generated
 * returns the number of locations that a list of seeds generates
 *
 */
int num_loc_generated(int** seedFreqMatrix, int seedMinLen, int** seedsList, int seedsListLen){
  int locations = 0;

  for(int i = 0; i < seedsListLen; i ++){
    locations += seedFreqMatrix[seedsList[i][LENGTH] - seedMinLen][seedsList[i][INDEX]];
  }

  return locations;
}



/**
 *
 * FUNCTION: is_valid
 * takes in a list of seeds and returns (true or false) if it
 * is a valid seed location. (e.g. no overlapping)
 *
 */
int is_valid(int readLen, int** seedsList, int seedsListLen){
  memset(validLocs, 1, readLen * sizeof(validLocs[0]));

  int currIndex;
  for(int i = 0; i < seedsListLen; i++){
    for(int j = 0; j < seedsList[i][LENGTH]; j++){
      currIndex = j + seedsList[i][INDEX];
      /*
       * return with 0 (i.e. false) if the index is already occupied
       */
      if(currIndex >= readLen || !validLocs[currIndex]) 
	return 0;
      /*
       * set the space as occupied if not already
       */
      else 
	validLocs[currIndex] = 0;
    }
  }
  
  return 1; //if none of the seed overlap or are otherwise invalid, return 1 (i.e. true)
}


/**
 *
 * FUNCTION: find_optimal_seeds 
 * returns a string of the optimal seed locations
 *
 */
char* find_optimal_seeds(PyObject* seed_freq_marix, int readLen, int seedMinLen, int seedMaxLen, int errorsAllowed, int printFreq){

  const int seedsListLen = errorsAllowed + 1;
  /*
   * Read in the seed_freq_matrix and convert to 2d int array for c computations
   */
  if(!PyList_Check(seed_freq_marix))
    return NULL;

  int numLines = PyList_Size(seed_freq_marix);
  int* seedFreqMatrix[numLines];

  for(int i = 0; i < numLines; i++){
    seedFreqMatrix[i] = malloc( numLines * sizeof(int));
    for(int j = 0; j < numLines; j++){
      seedFreqMatrix[i][j] = (int) PyFloat_AsDouble(PyList_GET_ITEM(PyList_GET_ITEM(seed_freq_marix, i), j));
    }
  }

  /*
   * Allocate memory for the helper array validLocs 
   * and the seedsList holder (global var)
   */
  validLocs = malloc(readLen * sizeof(int*));

  int* currSeedsList[seedsListLen];
  // set all seeds in the starting position
  for(int i = 0; i < seedsListLen; i++){
    currSeedsList[i] = malloc(2 * sizeof(int*));
    currSeedsList[i][LENGTH] = seedMinLen;
    currSeedsList[i][INDEX] = 0;
  }

  int* optimalSeedsList[seedsListLen];
  for(int i = 0; i < seedsListLen; i++){
    optimalSeedsList[i] = malloc(2 * sizeof(int*));
    optimalSeedsList[i][LENGTH] = seedMinLen;
    optimalSeedsList[i][INDEX] = 0; 
  }
  

  /*
   * begin iterating over all possible locations
   */
  const unsigned long combinations = pow((seedMaxLen - seedMinLen + 1) * readLen, seedsListLen);
  int optimalLocGen = 2147483647; // initialize to max value for signed int
  int innerLoopIterations;

  for(unsigned long i = 0; i < combinations + printFreq; i += printFreq){

    innerLoopIterations = (combinations < printFreq * i) ? printFreq * i - combinations : printFreq;

    if(printFreq != 0){
      printf("On iteration %lu of %lu\n", i, combinations);
    }

    for(int j = 0; j < innerLoopIterations; j++){
 
      /*
       * a new better configuration is stored when found
       */ 
      if(is_valid(readLen, currSeedsList, seedsListLen)){
	int currLocGen = num_loc_generated(seedFreqMatrix, seedMinLen, currSeedsList, seedsListLen);
	if(optimalLocGen > currLocGen){
	
	  optimalLocGen  = currLocGen;
	  copy_seeds_list(currSeedsList, optimalSeedsList, seedsListLen);

	}
      }
       
      /*
       * increment the seed to the next possible location
       * think of a clock when trying to understand this code
       */ 
      for(int i = 0; i < seedsListLen; i++){
	if(currSeedsList[i][INDEX] < readLen - 1){
	  currSeedsList[i][INDEX]++;
	  break;
	}
	else{
	  currSeedsList[i][INDEX] = 0;
	  if(currSeedsList[i][LENGTH] < seedMaxLen){
	    currSeedsList[i][LENGTH]++;
	    break;
	  }
	  else{
	    currSeedsList[i][LENGTH] = seedMinLen;
	  }
	}
      }

    }
  }

  free(validLocs);
  return seeds2string(optimalSeedsList, seedsListLen);

}
