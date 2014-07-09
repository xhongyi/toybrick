#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include "HashTable.h" 


#define KMER_LEN 4
//#define HASH_TABLE "mini_genome.fasta.ht"
#define HASH_TABLE "human_g1k_v37.fasta.1.ht"
#define SEED_FILE "seed.inp"

using namespace std;

int main(){

  HashTable ht(KMER_LEN);
  ht.generateHash("human_g1k_v37.fasta.1", "human_g1k_v37.fasta.1.ht");
  //ht.generateHash("mini_genome.fasta", "mini_genome.fasta.ht");
  ht.loadHash(HASH_TABLE);
  ht.loadChromo(0);

  FILE *fp;
  fp = fopen(SEED_FILE,"r");
  if( fp == NULL ){
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }

  while(1){

    char seed[1024];
    if(fgets(seed, sizeof(seed), fp) == NULL){
      break;
    }

    strtok(seed, "\n");
    int patternLength = strlen(seed);

    ht.queryKmer(seed);
    const unsigned int numLocs = ht.getLocLength();
  
    cout << "The number of location found: " << numLocs << endl;
    for(unsigned int i = 0; i < 5; i++){
      cout << ht.getLoc() << endl;
    }

  }

  return 0;

}
