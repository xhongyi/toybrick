
/*
 * Created by : John Emmons
 */

#include <iostream>
#include <stdio.h>
#include <nmmintrin.h>
#include <stdint.h>

#include "RefGenome.h" 

void print128_num(__m128i var){
  uint16_t *val = (uint16_t*) &var;
  printf("Numerical: %i %i %i %i %i %i %i %i \n", 
	 val[0], val[1], val[2], val[3], val[4], val[5], 
	 val[6], val[7]);
}

int main(int argc, char* argv[]){ 

  char my_genome[] = "genome.fasta";

  Reference my_ref(80);
  my_ref.build(my_genome);
  
  return 0;

}
