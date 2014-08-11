
/*
 * Created by : John Emmons
 */

#include <iostream>
#include <stdio.h>
#include <stdint.h>

#include "RefGenome.h" 

using namespace std;

int main(int argc, char* argv[]){ 

  //char my_genome[] = "human_g1k_v37.fasta";
  //char my_genome_db[] = "human_g1k_v37.fasta.db";
  
  char my_genome[] = "genome.fasta";
  char my_genome_db[] = "genome.fasta.db";


  Reference my_ref;
  my_ref.build(my_genome);
  my_ref.store();

  Reference my_other_ref;
  if(my_other_ref.load(my_genome_db)){
    cout << "There was an error!" << endl;
  }

  unsigned char* seed[2];
  my_ref.query(5, 10, seed);
 
  return 0;

}
