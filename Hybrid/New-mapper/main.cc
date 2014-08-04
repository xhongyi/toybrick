
/*
 * Created by : John Emmons
 */

#include <iostream>
#include <stdio.h>
#include <stdint.h>

#include "RefGenome.h" 

int is_big_endian(void)
{
  union {
    uint32_t i;
    char c[4];
  } e = { 0x01000000 };

  return e.c[0];
}

int main(int argc, char* argv[]){ 

  //char my_genome[] = "human_g1k_v37.fasta";
  //char my_genome_db[] = "human_g1k_v37.fasta.db";
  
  char my_genome[] = "genome.fasta";
  char my_genome_db[] = "genome.fasta.db";


  Reference my_ref;
  my_ref.build(my_genome);
  my_ref.store();

  Reference my_other_ref;
  my_other_ref.load(my_genome_db);

  unsigned char* seed[2];
  my_ref.query(5, 10, seed);
 
  printf("System is %s-endian.\n",
	 is_big_endian() ? "big" : "little");

 
  return 0;

}
