
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <list>

#include "RefGenome.h"
#include "bit_convert.h"
#include "print.h"

#define SSE_BITCONVERT_STRING_SIZE 128
#define DEFAULT_FASTA_LINE_WIDTH 80
#define BITS_PER_BYTE 8
#define EXTRA_CHARS 3
#define NULL_CHAR 1

#define __aligned__ __attribute__((aligned(16)))

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
  /*  delete reference.bits0;
      delete reference.bits1;*/
}


/**
 * Used in the public "build" function. Takes in a fasta file name 
 *
 * Sets num_genome_breaks, num_base_pairs, and allocates and sets
 * the genome_breaks vector
 */
void Reference::collect_metadata(char* ref_file_name){

  /* open fasta file */
  FILE *ref_file_ptr;
  ref_file_ptr = fopen(ref_file_name,"r");

  if(ref_file_ptr == NULL){
    perror("Error while opening the file");
    exit(EXIT_FAILURE);
  }

  /* loop through fasta file and collect metadata */
  char line[fasta_line_width];
  while(fgets(line, sizeof(line), ref_file_ptr)){

    switch(line[0]){
    case '>':
      name_genome_breaks.push_back(string(line));
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

}

/**
 * Replaces 'N' by 'A' in char array. Used to remove unknown base pairs  
 * in the reference
 *
 * Also, remove newline char at the end if it exists
 *
 * Returns the length of the reformatted string
 */
int Reference::reformat_line(char* buffer){
  const int len = strlen(buffer);
  for(int i = 0; i < len; i++){
    if(buffer[i] == 'N'){ buffer[i] = 'A'; }
  }

  /* remove newline at end of line if it exists */
  const int end = len - 1;
  if(buffer[end] == '\n') { 
    buffer[end] = '\0'; 
    return end;
  }
  else{ return len; }
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

  /* open fasta file */
  FILE *ref_file_ptr;
  ref_file_ptr = fopen(ref_file_name,"r");

  if(ref_file_ptr == NULL){
    perror("Error while opening the file");
    exit(EXIT_FAILURE);
  }

  /* Declare helper variables and buffers for bit conversion */
  int str_len, str_pos, buffer_pos = 0, ref_pos = 0;
  const int bits_per_sse_convert = SSE_BITCONVERT_STRING_SIZE / BITS_PER_BYTE;
  char char_buffer[SSE_BITCONVERT_STRING_SIZE + NULL_CHAR] = "\0";

  /* loop through fasta file and convert basepairs to bits */
  char line[fasta_line_width];
  while(fgets(line, sizeof(line), ref_file_ptr)){

    switch(line[0]){
    case '>':
    case ';':
      break; /* skip non-base pair lines */
    case 'A':
    case 'C':
    case 'G':
    case 'T':
    case 'N': 
      /* 
       * replaces 'N' base pair (unknowns) with A's 
       * remove newline at end if present
       * returns length of char*
       */
      str_len = reformat_line(line);
      
      /*
       * If TRUE: The entire line will fit into the dedicated bit conversion
       * buffer. Will read more lines until the buffer is filled (i.e has
       * SSE_BITCONVERT_STRING_SIZE chars)
       *
       * If FALSE: The line will fill the dedicated buffer. All the chars that
       * will fit into the buffer are copied, then the buffer is converted into
       * bits and stored into the reference database. After converting, the
       * chars that did not fit into the buffer are copied into the buffer to
       * which is emptied in the process.
       */
      if(buffer_pos + str_len < SSE_BITCONVERT_STRING_SIZE){
	strncat(char_buffer, line, str_len);
	buffer_pos += str_len;
      }
      else{
	/* figure out how much of the current line can fit into the buffer */
	str_pos = SSE_BITCONVERT_STRING_SIZE - buffer_pos;
	strncat(char_buffer, line, str_pos);

	/* convert the buffer to bits and copy to database */
      	//printf("\n");
	//printf("%s\n", char_buffer);
	sse3_convert2bit1(char_buffer, reference.bits0 + ref_pos, reference.bits1 + ref_pos);
	//printbytevector(reference.bits0 + ref_pos, 16);
	//printf("\n");
	//printbytevector(reference.bits1 + ref_pos, 16);
	//printf("\n");
	ref_pos += bits_per_sse_convert;

	/* copy the remainder of the line to the next buffer */
	strncpy(char_buffer, line + str_pos, str_len - str_pos + 1); 
	buffer_pos = str_len - str_pos;
      }
      break;
    default:
      fprintf(stderr, "Unknown character found in %s\n", ref_file_name);
      exit(EXIT_FAILURE);
    }    
  }
  /*
   * Convert any remaining chars in the buffer to bits and put into db
   */
  printf("%s\n", char_buffer);
  sse3_convert2bit1(char_buffer, reference.bits0 + ref_pos, reference.bits1 + ref_pos);
  printbytevector(reference.bits0 + ref_pos, 16);                                                                                                           
  printf("\n");                                                                                                                                             
  printbytevector(reference.bits1 + ref_pos, 16);                                                                                                           
  printf("\n");

  num_genome_breaks = genome_breaks.size();
  fclose(ref_file_ptr);
 
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
