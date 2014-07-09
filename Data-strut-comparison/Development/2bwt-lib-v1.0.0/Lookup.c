#include <stdio.h>#include <stdlib.h>#include <unistd.h>#include "2BWT-Interface.h"int main(int argc, char *argv[]) {  /*   * Check and parse CLI inputs   */   if ( argc != 3 ){    printf("\nusage: %s <*.fasta> <seed.inp>\n\n", argv[0] );    return 1;  }  const char * FASTA_FILE = argv[1];  const char * SEED_FILE = argv[2];  /*   * Append ".index" and ".index.sa" to .fasta file to load the 2-BWT   */  int fasta_len = strlen(FASTA_FILE);  char INDEX_FILE[fasta_len + 6];  strncpy(INDEX_FILE, FASTA_FILE, fasta_len);  INDEX_FILE[fasta_len] = '\0';  strncat(INDEX_FILE, ".index", 6);    int index_len = strlen(INDEX_FILE);  char SA_FILE[index_len + 3];  strncpy(SA_FILE, INDEX_FILE, index_len);  SA_FILE[index_len] = '\0';  strncat(SA_FILE, ".sa", 3);  /*   * Check if the Suffix Array file has been generated   */  if( access(SA_FILE, F_OK) == -1 ){    printf("\n%s file was not found!\n", SA_FILE);    printf("To generate it: ./2bwt-builder %s \n\n", FASTA_FILE);    return 1;  }  /*   * Declare helper variables   */  int i,j,k,c;  int index;  /*   * Declared variables for BWT search   */  unsigned int l,r,rev_l,rev_r;  unsigned int result_l[ALPHABET_SIZE];  unsigned int result_r[ALPHABET_SIZE];  unsigned int result_rev_l[ALPHABET_SIZE];  unsigned int result_rev_r[ALPHABET_SIZE];      /*   * Load the 2-BWT   */  printf("Loading index (%s) ... ", FASTA_FILE); fflush(stdout);  Idx2BWT * idx2BWT = BWTLoad2BWT(INDEX_FILE,".sa");  printf("DONE\n\n");   /*   * Open SEED_FILE  and check that it opened successfully   */  FILE *fp;  fp = fopen(SEED_FILE,"r");  if( fp == NULL ){    perror("Error while opening the file.\n");    exit(EXIT_FAILURE);  }  /*   * Loop through all seeds in SEED_FILE   */  while(1){    //Variables for outputting result    unsigned int offset;    int sequenceId;    unsigned int saCount;        /*     * The buffer that holds the seeds from SEED_FILE     */    char seed[1024];        /*    * IMPORTANT: This is how we exit the loop    * Break out if at the end of the file or a blank line    */    if(fgets(seed, sizeof(seed), fp) == NULL){      break;    }    /*     * Strip the newline and record the seed     */    strtok(seed, "\n");    int seedLength = strlen(seed);    /*     * Convert the seed into 2BWT recognised coding scheme     */    BWTConvertPattern(idx2BWT,seed,seedLength,seed);        // The following performs a bi-directional search of the seed    // Starting from the middle of the seed, first move right, then move left.    // ===================================================================================    // |    j = seedLength / 2;    BWTSARangeInitial(idx2BWT,seed[j],&l,&r);    BWTSARangeInitial(idx2BWT,seed[j],&rev_l,&rev_r);    for (i=j+1;i<seedLength;i++) {      BWTSARangeForward_Bidirection(idx2BWT,seed[i],&l,&r,&rev_l,&rev_r);    }    for (i=j-1;i>=0;i--) {      BWTSARangeBackward_Bidirection(idx2BWT,seed[i],&l,&r,&rev_l,&rev_r);    }    printf("SA Range being = %u %u %u %u (%u)\n\n",l,r,rev_l,rev_r,r-l+1);    // |    // ===================================================================================            // output the results    // ===================================================================================    // |    // |    j=r-l+1;    for (i=0;i<j;i++) {      BWTRetrievePositionFromSAIndex(idx2BWT,l+i,&sequenceId,&offset);      //printf("Occurrence found in sequence #%d with offset %u\n",sequenceId,offset);      //      fprintf(output, "Occurrence found in sequence #%d with offset %u\n",sequenceId,offset);    }    // |    // |    // ===================================================================================  }      /*   * Free up the 2BWT index   */  printf("\nFree index ... ");   fflush(stdout);  BWTFree2BWT(idx2BWT);  printf("DONE\n");       return 0;}