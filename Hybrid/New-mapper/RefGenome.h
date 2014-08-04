
/*
 * Genome Reference
 * Used to build, store, load, and access a bit-vector DNA database
 *
 * Created by : John Emmons
 * Updated on : Aug 4, 2014
 */

#ifndef REF_GENOME_H
#define REF_GENOME_H

#include <vector>
#include <stdint.h>

/* Class definition */
class Reference{

  struct RefDB{
    unsigned char *bits0;
    unsigned char *bits1;
  };

 private:
  RefDB reference;

  unsigned int num_genome_breaks;
  std::vector<unsigned long long> genome_breaks;
  std::vector<std::string> name_genome_breaks;

  unsigned long long num_base_pairs;

  std::string fasta_file_name;
  const unsigned int fasta_line_width;

  bool previously_loaded_or_built;
  
  /**
   * Used to change the internal state of the object. Records
   * whether the build or load functions have been called. 
   *
   * @param void
   * @return void
   */
  void check_if_already_loaded_or_built();

  /**
   * Used in the public "build" function. Takes in a fasta file name
   *
   * Sets num_genome_breaks, num_base_pairs, and allocates and sets
   * the genome_breaks vector
   *
   * @param name of *.fasta file to read
   * @return success or failure
   */
  bool collect_metadata(char*);
  
  /**
   * Replaces 'N' by 'A' in char array. Used to remove unknown base pairs
   * in the reference. Also, removes the newline character at the end of 
   * the line if it exists
   *
   * @param a c-string (may be mutated by function)
   * @return length of the edited string (may have changed)
   */ 
  int reformat_line(char*);

 public:
  /** constructors and destructor */
  Reference();
  Reference(unsigned int);
  ~Reference();

  /**
   * Builds a reference database from fasta file. Converts to bits internally 
   * 
   * @param name of *.fasta file to read
   * @return success or failure
   */
  bool build(char*);

  /**
   * Stores a reference database that has been previously loaded or built 
   *
   * @param void
   * @return success or failure
   */
  bool store();

  /**
   * Loads a stored reference database file. Input SHOULD have .db extension 
   * 
   * @param c-string of the reference database name stored on disk
   * @return success or failure
   *
   */
  bool load(char*);
  
  /**
   * Interface for accessing contiguous segments of base pairs stored in 
   * reference database. 
   *
   * @param position in database, length of segment, 2-element array
   * of unsigned chars pointers. 
   * @return success or failure
   *
   * NOTE: the unsigned char pointers WILL BE CHANGED to point at memory 
   * allocated by the function which contains the bit-vector representation 
   * of the base pairs. YOU ARE RESPONSIBLE FOR 'DELETE'ing the memory.
   */
  bool query(unsigned long long, unsigned long long, unsigned char**);

};

#endif /* _REF_GENOME_  */
