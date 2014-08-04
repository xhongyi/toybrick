
/*
 * Written by : John Emmons
 * Contact : emmons.john@gmail.com
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <list>

#include "RefGenome.h"
#include "bit_convert.h"
#include "print.h"

/*
 * Define constants
 */
#define SSE_BITCONVERT_STRING_SIZE 128
#define DEFAULT_FASTA_LINE_WIDTH 80
#define BITS_PER_BYTE 8
#define EXTRA_CHARS 3 /* the space a newline, carriage return, and null char take up */
#define NULL_CHAR 1 /* space the null char takes up */

#define DB_FILE_EXTENSION ".db"
#define META_FILE_EXTENSION ".meta"

#define META_FILE_WIDTH 1024

using namespace std;

/**
 * Default Constructor: assumes no more than 80 basepairs per line in
 * fasta file (i.e. set fasta_line_width = 80)
 *
 * Also initializes class variables
 */
Reference::Reference() : fasta_line_width(DEFAULT_FASTA_LINE_WIDTH){ 
  num_base_pairs = 0;
  num_genome_breaks = 0;
  previously_loaded_or_built = false;
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
  num_genome_breaks = 0;
  previously_loaded_or_built = false;
}

/**
 * Destructor
 */
Reference::~Reference(){ 
  free(reference.bits0);
  free(reference.bits1);
}

void Reference::check_if_already_loaded_or_built(){
  
  /* ensures the build or load function are only called once per reference object */
  if(previously_loaded_or_built){
    fprintf(stderr, "Error: a reference object may only be built or loaded once per \
 instance. Check to make sure you only made one call to either 'load' or 'build'.\n");
    exit(EXIT_FAILURE);
  }
  else{
    previously_loaded_or_built = true;
  }

}

/**
 * Used in the public "build" function. Takes in a fasta file name 
 *
 * Sets num_genome_breaks, num_base_pairs, and allocates and sets
 * the genome_breaks vector
 */
void Reference::collect_metadata(char* ref_file_name){
  
  /* save the filename */
  fasta_file_name = string(ref_file_name);

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

  /* ensures the build function is only called once per reference object */
  check_if_already_loaded_or_built();
  
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
  const unsigned long long bytes2allocate = (num_base_pairs + (BITS_PER_BYTE - 1)) / BITS_PER_BYTE;
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
  const int bits_per_sse_convert = SSE_BITCONVERT_STRING_SIZE / BITS_PER_BYTE;
  int str_len, str_pos, buffer_pos = 0, ref_pos = 0;

  char char_buffer[SSE_BITCONVERT_STRING_SIZE + NULL_CHAR] = "\0";
  char line[fasta_line_width];
  
  /* loop through fasta file and convert basepairs to bits */
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
       * chars that did not fit into the buffer are copied into the buffer
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

	/* convert the buffer to bits and store in database */
	sse3_convert2bit1(char_buffer, reference.bits0 + ref_pos, reference.bits1 + ref_pos);
	ref_pos += bits_per_sse_convert;

	/* empty the buffer and copy the remainder of the line into the buffer */
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
   * Convert any remianing base pairs into bits once the file has ended
   * and copy them to the database. Function sse3_convert2bit1 converts
   * SSE_BITCONVERT_STRING_SIZE chars at a time, so to prevent accessing
   * unallocated memory in the reference, bits are stored in tmp arrays
   * then copied in the loop into the reference database.
   */
  unsigned char tmp_bits0[bits_per_sse_convert];
  unsigned char tmp_bits1[bits_per_sse_convert];  
  const int num_char_to_print = (buffer_pos + (BITS_PER_BYTE - 1)) / BITS_PER_BYTE;

  sse3_convert2bit1(char_buffer, tmp_bits0, tmp_bits1);

  for(int i = 0; i < num_char_to_print; i++){
    reference.bits0[ref_pos + i] = tmp_bits0[i];
    reference.bits1[ref_pos + i] = tmp_bits1[i];
  }

  /* close the fasta file */
  fclose(ref_file_ptr);

  //print out the contents of the database raw
  /*
  for(int i = 0; i < num_base_pairs / 8 ; i += 16){
    printbytevector(reference.bits0 + i, 16);
    printf("\n");
    printbytevector(reference.bits1 + i, 16);  
    printf("\n");
  }
  */

}

/**
 * writes the contents of the reference database to a file <ref_file_name>.db
 * and stores metadata in a separate file <refdb_file_name>.db.meta
 * 
 */
void Reference::store(){
  
  /* open and store metadata file */
  FILE *refmeta_file_ptr;
  refmeta_file_ptr = fopen((fasta_file_name + DB_FILE_EXTENSION + META_FILE_EXTENSION).c_str(),"w");

  if(refmeta_file_ptr == NULL){
    perror("Error while opening the file");
    exit(EXIT_FAILURE);
  }

  /* print metadata */
  fprintf(refmeta_file_ptr, "%llu\n", num_base_pairs);
  fprintf(refmeta_file_ptr, "%u\n", num_genome_breaks);
  for(int i = 0; i < num_genome_breaks; i++){
    fprintf(refmeta_file_ptr, "%llu\n", genome_breaks[i]);
    fprintf(refmeta_file_ptr, "%s", name_genome_breaks[i].c_str());
  }
  fprintf(refmeta_file_ptr, "\n");

  fclose(refmeta_file_ptr);

  /* open fasta database output file */
  FILE *refdb_file_ptr;
  refdb_file_ptr = fopen((fasta_file_name + DB_FILE_EXTENSION).c_str(),"wb");

  if(refdb_file_ptr == NULL){
    perror("Error while opening the file");
    exit(EXIT_FAILURE);
  }
  
  /* figure out how many byte must be written */
  const unsigned long long bytes2write = (num_base_pairs + (BITS_PER_BYTE - 1)) / BITS_PER_BYTE;

  /* write the bit array corresonding to the first bit of each basepair */
  fwrite(reference.bits0 , sizeof(unsigned char), bytes2write, refdb_file_ptr);

  /* write the bit array corresonding to the second bit of each basepair */
  fwrite(reference.bits1 , sizeof(unsigned char), bytes2write, refdb_file_ptr);
  
  fclose(refdb_file_ptr);

}

/**
 *
 */
void Reference::load(char* refdb_file_name){

  /* ensures the load function is only called once per reference object */
  check_if_already_loaded_or_built();

  /* open and read metadata file */
  FILE *refmeta_file_ptr;
  refmeta_file_ptr = fopen((string(refdb_file_name) + META_FILE_EXTENSION).c_str(), "r");
  
  if(refmeta_file_ptr == NULL){
    perror("Error while opening the file");
    exit(EXIT_FAILURE);
  }

  /* read the metadata file and collect metadata */
  char line[fasta_line_width];

  /* read number of base pairs in db file */
  if( !fgets(line, sizeof(line), refmeta_file_ptr) ) { 
    fprintf(stderr,"Error readng metadata file\n"); exit(EXIT_FAILURE); }
  num_base_pairs = atol(line);//SHOULD BE CHANGED TO CONVERT TO UNSIGNED LONG LONG

  /* read number of genome division in db */
  if( !fgets(line, sizeof(line), refmeta_file_ptr) ) { 
    fprintf(stderr,"Error readng metadata file\n"); exit(EXIT_FAILURE); }
  num_genome_breaks = atoi(line);//SHOULD BE CHANGED TO CONVERT TO UNSIGNED INT

  for(unsigned int i = 0; i < num_genome_breaks; i++){
    /* read the position of the genome division */
    if( !fgets(line, sizeof(line), refmeta_file_ptr) ) { 
      fprintf(stderr,"Error readng metadata file\n"); exit(EXIT_FAILURE); }
    genome_breaks.push_back(atol(line));//SHOULD BE CHANGED TO CONVERT TO UNSIGNED LONG LONG

    /* read the name of the genome */
    if( !fgets(line, sizeof(line), refmeta_file_ptr) ) { 
      fprintf(stderr,"Error readng metadata file\n"); exit(EXIT_FAILURE); }
    name_genome_breaks.push_back(string(line));
    
  }

  fclose(refmeta_file_ptr);

  /*
    printf("%u %u %u %s", num_base_pairs, num_genome_breaks, genome_breaks[0], name_genome_breaks[0].c_str());
   */
 
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
   * NOTE: because of 
   *
   * so 8 bps fit into a single 8 bit byte. We first add 7 then 
   * divide by 8 to ensure enough memory is allocated.
   */  
  const unsigned long long bytes2allocate = (num_base_pairs + (BITS_PER_BYTE - 1)) / BITS_PER_BYTE;
  reference.bits0 = (unsigned char*) malloc(bytes2allocate * sizeof(unsigned char));
  reference.bits1 = (unsigned char*) malloc(bytes2allocate * sizeof(unsigned char));

  /* open and read in binary reference database */
  FILE *refdb_file_ptr;
  refdb_file_ptr = fopen(refdb_file_name, "rb");
  
  if(refdb_file_ptr == NULL){
    perror("Error while opening the file");
    exit(EXIT_FAILURE);
  }
  
  /* read the first bits into the database */
  if( !fread(reference.bits0, sizeof(*reference.bits0), bytes2allocate, refdb_file_ptr) ){
    fprintf(stderr,"Error reading reference database (bits 0)\n"); exit(EXIT_FAILURE); }

  /* read the second bits into the database */
  if( !fread(reference.bits1, sizeof(*reference.bits0), bytes2allocate, refdb_file_ptr) ){
    fprintf(stderr,"Error reading reference database (bits 1)\n"); exit(EXIT_FAILURE); }

  /*
  printf("\n\n\n");
  for(int i = 0; i < num_base_pairs / 8 ; i += 16){
    printbytevector(reference.bits0 + i, 16);
    printf("\n");
    printbytevector(reference.bits1 + i, 16);  
    printf("\n");
    }*/

}

/**
 * Takes in the global position of a seed and the seend length. Then returns the two pointers to bit vectors corrsponding to that seed. 
 *
 * THE USER IS RESPONSIBLE FOR FREEING MEMORY CREATED BY THIS FUNCTION
 */
void Reference::query(unsigned long long pos, unsigned long long length, unsigned char** seed){

  /* The byte where the seed begins in the reference bitvector */
  const unsigned long long ref_pos = pos / BITS_PER_BYTE;

  /* The bit within the start byte where the seed begins */
  const unsigned int ref_shift = pos % BITS_PER_BYTE;

  /* Bytes to alloacte for the output arrays */
  const unsigned long long bytes2allocate = (length + (BITS_PER_BYTE - 1)) / BITS_PER_BYTE;
  
  /* Declare a temporary array to store the pointers to the bit vectors */
  unsigned char* tmp_seed[2];
  
  /* allocates memory using C++ "new". The user of the function must deallocate memory with "delete" */
  tmp_seed[0] = new unsigned char[bytes2allocate]; //(char*) malloc(bytes2allocate * sizeof(unsigned char));
  tmp_seed[1] = new unsigned char[bytes2allocate]; //(char*) malloc(bytes2allocate * sizeof(unsigned char));

  /* store the bits into the reference byte-by-byte */
  unsigned char small_buff0, small_buff1;
  for(int i = 0; i < bytes2allocate; i++){
  
    small_buff0 = reference.bits0[ref_pos + i];
    small_buff1 = reference.bits1[ref_pos + i];
    
    small_buff0 >>= ref_shift;
    small_buff1 >>= ref_shift;
    
    small_buff0 |= reference.bits0[ref_pos + i + 1] << (BITS_PER_BYTE - ref_shift);
    small_buff1 |= reference.bits1[ref_pos + i + 1] << (BITS_PER_BYTE - ref_shift);
    
    tmp_seed[0][i] = small_buff0;
    tmp_seed[1][i] = small_buff1;
  }

  /* Make any trailing but not included in the seed zeros */
  tmp_seed[0][bytes2allocate - 1] &= 0xff >> BITS_PER_BYTE - length % BITS_PER_BYTE;
  tmp_seed[1][bytes2allocate - 1] &= 0xff >> BITS_PER_BYTE - length % BITS_PER_BYTE;
  

  printbytevector(tmp_seed[0], bytes2allocate);
  cout << endl;  
  printbytevector(tmp_seed[1], bytes2allocate); 
  cout << endl;

  /* Copy the bit vectors to the output */
  seed[0] = tmp_seed[0];
  seed[1] = tmp_seed[1];
}
