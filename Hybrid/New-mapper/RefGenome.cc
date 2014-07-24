
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <list>

#include "RefGenome.h"
#include "bit_convert.h"

#define DEFAULT_FASTA_LINE_WIDTH 80
#define EXTRA_CHARS 3

using namespace std;

/**
 * Default Constructor: assumes no more than 80 basepairs per line in
 * fasta file (i.e. set fasta_line_width = 80)
 *
 * Also initializes class variables
 */
Reference::Reference() : fasta_line_width(DEFAULT_FASTA_LINE_WIDTH){ 
  num_base_pairs = 0;
}

/**
 * Constructor: OVERRIDES THE DEFAULT FASTA LINE WIDTH OF 80 CHARACTERS!
 * Sets the constant file width which is the number of basepairs per line
 * in the fasta file plus 3 more for newline, carriage return, and null
 *
 * Also initializes class variables
 */
Reference::Reference(unsigned int f_f_width) : fasta_line_width(f_f_width + EXTRA_CHARS) {  
  num_base_pairs = 0;
}

/**
 * Destructor
 */
Reference::~Reference(){ 
  delete reference.bits0;
  delete reference.bits1;
}


/**
 * Used in the public "build" function. Takes in a fasta file name 
 *
 * Sets num_genome_breaks, num_base_pairs, and allocates and sets
 * the genome_breaks vector
 */
void Reference::collect_metadata(char* ref_file_name){

  FILE *ref_file_ptr;
  ref_file_ptr = fopen(ref_file_name,"r");

  if(ref_file_ptr == NULL){
    perror("Error while opening the file");
    exit(EXIT_FAILURE);
  }

  char line[fasta_line_width];
  while(fgets(line, sizeof(line), ref_file_ptr)){

    switch(line[0]){
    case '>':
      genome_breaks.push_back(num_base_pairs);
      break;
    case ';':
      break; /* ';' is a comment line */
    case 'A':
    case 'C':
    case 'G':
    case 'T':
    case 'N':
      num_base_pairs += (strlen(line) - 1); /* subtract newline from count */
      break;
    default:
      fprintf(stderr, "Unknown character found in %s\n", ref_file_name);
      exit(EXIT_FAILURE);
    }

  }  

  num_genome_breaks = genome_breaks.size();

  fclose(ref_file_ptr);
  delete ref_file_ptr;

}


/**
 * Takes in fasta reference file name and builds the reference database
 */ 
void Reference::build(char* ref_file_name){
  
  /* sets num_genome_breaks, num_base_pairs, and genome_breaks */
  collect_metadata(ref_file_name);

  /* 
   * allocate memory for genome database 
   *
   * base pairs (bp) are stored in 2 bits each (00, 01, 10, 11)
   * the bits for bps are split into two arrays (bits0, bits1)
   *
   * For example, ATCCAAGTA = 00,11,01,01,00,00,10,11,00 but the
   * bits for each are split into two array
   *         A T C C A A G T A
   *         | | | | | | | | |
   *         V V V V V V V V V 
   * bits0 = 0 1 0 0 0 0 1 1 0
   * bits1 = 0 1 1 1 0 0 0 1 0
   *
   * so 8 bps fit into a single 8 bit byte. We first add 7 then 
   * divide by 8 to ensure enough memory is allocated.
   */  
  unsigned long long bytes2allocate = (num_base_pairs + 7) / 8;
  reference.bits0 = (unsigned char*) malloc(bytes2allocate * sizeof(unsigned char));
  reference.bits1 = (unsigned char*) malloc(bytes2allocate * sizeof(unsigned char));
  
}

/**
 *
 */
void Reference::store(){

}

/**
 *
 */
void Reference::load(char* refdb_file_name){

}

/**
 *
 */
void Reference::query(unsigned long long pos, char** seed){

  printf("hello world\n");

}
