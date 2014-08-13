#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "HashTable.h" 

//#define OUTPUT "ht_lookup.out"
#define OUTPUT "/dev/null"
#define TIME_FILE "time.out"

using namespace std;

int main(int argc, char *argv[]){

  /*
   * Check and parse CLI inputs
   */
  if ( argc != 4 ){
    printf("\nusage: %s <*.fasta> <seed.inp> <seed length>\n\n", argv[0] );
    return 1;
  }

  const char * FASTA_FILE = argv[1];
  const char * SEED_FILE = argv[2];
  const int seed_len = atoi(argv[3]);

  if( access(FASTA_FILE, F_OK) == -1 ){
    printf("\n%s file was not found!\n\n", FASTA_FILE);
    return 1;
  }

  /*
   * Check if the hash table file has been generated
   */
  int fasta_len = strlen(FASTA_FILE);
  char HT_FILE[fasta_len + 3];
  strncpy(HT_FILE, FASTA_FILE, fasta_len);
  HT_FILE[fasta_len] = '\0';
  strncat(HT_FILE, ".ht", 3);

  HashTable ht(seed_len);
  if( access(HT_FILE, F_OK) == -1 ){
    printf("\n%s file was not found!\n", HT_FILE);
    printf("I am autogenerating it.\n\n");
    ht.generateHash(FASTA_FILE, HT_FILE);
  }
     
  /*
   * Load the hash table file
   */
  printf("\nHash table loading (%s) ... ht.loadHash(HT_FILE);\n\n", HT_FILE); fflush(stdout);
  ht.loadHash(HT_FILE);
  ht.loadChromo(0);
  printf("Done ...\n\n"); fflush(stdout);

  FILE *fp;
  fp = fopen(SEED_FILE,"r");
  if( fp == NULL ){
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }

  FILE *fo;
  fo = fopen(OUTPUT,"w");
  if( fo == NULL ){
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }

  FILE *time;
  time = fopen(TIME_FILE,"w");
  if( time == NULL ){
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }
  
  /*
   * Create timer
   */
  clock_t start, stop;
  float elapsedTime = 0;

  /*
   * Loop through all seeds in SEED_FILE
   */
  char seed[1024];
  start = clock();
  while(fgets(seed, sizeof(seed), fp) != NULL){

    strtok(seed, "\n");
    ht.queryKmer(seed);
    const unsigned int numLocs = ht.getLocLength();
  
    /*
     * Output the results
     */
    fprintf(fo, "#%d\n", numLocs);
    for(unsigned int i = 0; i < numLocs; i++){
      fprintf(fo, "%u\n", ht.getLoc());
    }
    fprintf(fo, "-\n");
    fflush(fo);
  }
  stop = clock();

  /*
   * Close input and output files
   */
  fclose(fp);
  fclose(fo);

  /*
   * Output the time
   */
  elapsedTime = (float)(stop - start) / (float)CLOCKS_PER_SEC * 1000.0f;
  fprintf(time, "time (ms): %f\n", elapsedTime);
  fclose(time);

  return 0;

}
