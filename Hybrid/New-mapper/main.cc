
/*
 * Created by : John Emmons
 */

#include <iostream>
#include <stdio.h>
#include <stdint.h>

#include "RefGenome.h" 

int main(int argc, char* argv[]){ 

  //char my_genome[] = "human_g1k_v37.fasta";
  char my_genome[] = "genome.fasta";

  Reference my_ref;
  my_ref.build(my_genome);
  
  return 0;

}
