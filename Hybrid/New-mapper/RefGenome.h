
/*
 * Created by : John Emmons
 */

#ifndef REF_GENOME_H
#define REF_GENOME_H

#include <vector>
#include <stdint.h>

class Reference{

  struct RefDB{
    unsigned char *bits0;
    unsigned char *bits1;
  };

 private:
  RefDB reference;

  unsigned int num_genome_breaks;
  std::vector<std::string> name_genome_breaks;
  std::vector<unsigned long long> genome_breaks;
  
  unsigned long long num_base_pairs;

  const unsigned int fasta_line_width;
  
  /**
   * Used in the public "build" function. Takes in a fasta file name
   *
   * Sets num_genome_breaks, num_base_pairs, and allocates and sets
   * the genome_breaks vector
   */
  void collect_metadata(char*);
  
 public:
  Reference();
  Reference(unsigned int);
  ~Reference();

  /**
   * Takes in fasta reference file name and builds the reference database 
   */
  void build(char*);

  /**
   * Stores the reference database as a file. Output is <reference>.fasta.db 
   */
  void store();

  /**
   * Loads a stored reference database file. Input SHOULD have .db extension 
   */
  void load(char*);
  
  /**
   * Takes in the global position of the 
   */
  void query(unsigned long long, char**);

};

#endif /* _REF_GENOME_  */
