#!/usr/bin/python
import os

"""
This range is inclusive
"""
MIN_SEED_LEN = 5
MAX_SEED_LEN = 20
NUM_SEEDS = 1000

"""
Input file names
"""
GENOME_FILE = "human_g1k_v37.fasta.1" 
SEED_FILE = "seed.inp"

TIME_OUTPUT = "time.out"
OUTPUT_FILE = "ds_compare.out"

"""
Open output results file and print header
"""
time_results = open(OUTPUT_FILE, 'w')
time_results.write("seed_len\tBWT\tHT\n")
time_results.flush()

for seed_len in range(MIN_SEED_LEN, MAX_SEED_LEN + 1):
    """
    Print the seed_len
    """
    time_results.write(str(seed_len) + '\t')
    time_results.flush()

    """
    Create the seeds file (NUM_SEEDS seeds of length seed_len)
    """
    print "|\n--- CREATING SEEDS FILE ---\n|\n"
    os.system("rm -f " + SEED_FILE)
    os.system("./make_seeds.py " + str(seed_len) + " " + str(NUM_SEEDS))
    print NUM_SEEDS, "length", seed_len, "seeds were created"
    
    """
    Run the BWT code and record the time
    """
    print "|\n--- RUNNING BWT CODE ---\n|\n"
    os.system("rm -f " + TIME_OUTPUT)
    os.system("./sa_lookup " + GENOME_FILE + " " + SEED_FILE)
    time_file = open(TIME_OUTPUT, 'r')
    time_text = time_file.read()
    time_ms = float(time_text.split(" ")[-1])
    time_results.write(str(time_ms) + '\t')
    time_results.flush()

    """
    Copy the *.ht.<seed_len> file if possible
    """
    if(os.path.isfile(GENOME_FILE + ".ht." + str(seed_len))):
        print "|\n--- COPYING SAVED HT FILE ---\n|\n"
        use_name = GENOME_FILE + ".ht"
        store_name = GENOME_FILE + ".ht." + str(seed_len)
        os.system("mv " + store_name + " " + use_name)    
    else:
        print "|\n--- HT FILE NOT FOUND ---\n|\n"

    """
    Run the HT code and record the time
    """
    print "|\n--- RUNNING HT CODE ---\n|\n"
    os.system("rm -f " + TIME_OUTPUT)
    os.system("./ht_lookup " + GENOME_FILE + " " + SEED_FILE + " " + str(seed_len))
    time_file = open(TIME_OUTPUT, 'r')
    time_text = time_file.read()
    time_ms = float(time_text.split(" ")[-1])
    time_results.write(str(time_ms) + '\n')
    time_results.flush()

    """
    Save the *.ht file if possible
    """
    print "|\n--- SAVING THE HT FILE ---\n|\n"
    if(os.path.isfile(GENOME_FILE + ".ht")):
        use_name = GENOME_FILE + ".ht"
        store_name = GENOME_FILE + ".ht." + str(seed_len)
        os.system("mv " + use_name + " " + store_name)

os.system("rm -f " + TIME_OUTPUT)
time_results.close()
