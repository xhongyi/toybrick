#!/usr/bin/python
import os
import sys

# seed constans
SEED_LEN_MIN = 5
SEED_LEN_MAX = 20

if(len(sys.argv) != 2):
    print """
USAGE:
>$ python hash_table_generator.py <input_genome.fasta>
"""
    exit(1)
 
# hash table constants                                                                                                                                                                       
REF_GENOME_NAME = sys.argv[1]

print "GENERATING HASH TABLES FROM REFERNECE READ:", REF_GENOME_NAME

hash_log = open("hash.log", 'w')
for i in range(SEED_LEN_MIN,SEED_LEN_MAX + 1):
    print "\nNow working on Seed: %i" % i;
    hash_log.write('\n***SeedLength: %i\n' % i);
    hash_log.flush();
    os.system("./LongHashTable" + " " + REF_GENOME_NAME + " " + ( REF_GENOME_NAME + ".ht." + str(i) ) + " " +  str(i));
hash_log.close()


