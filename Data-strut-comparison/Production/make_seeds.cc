#include <iostream>
#include <stdlib.h>
#include <fstream>

#define SEED_FILE "seed.inp"

using namespace std;

int main(int argc, char* argv[]){
  if (argc != 3) {
    printf("\nUsage: %s <seed length> <num seeds>\n\n", argv[0]);
    return 1;
  }

  const char ALPHABET[] = {'A', 'C', 'G', 'T'};
  const int SEED_LENGTH = atoi(argv[1]);
  const int NUM_SEEDS = atoi(argv[2]);

  /*
   * Open SEED_FILE and check that it opened successfully
   */
  FILE *seed_output;
  seed_output = fopen(SEED_FILE,"w");

  if( seed_output == NULL ){
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }

  char seed[SEED_LENGTH + 1];
  for(unsigned int i = 0; i < NUM_SEEDS; i++){
    for(int j = 0; j < SEED_LENGTH; j++){
      seed[j] = ALPHABET[rand() % 4];
    }
    seed[SEED_LENGTH] = '\0';
    fprintf(seed_output, "%s\n", seed);
  }
  fclose(seed_output);
}
