#!/usr/bin/python
import os
import fast_brute

# read constant
NUM_READS = 1 # change to 30
READ_LEN = 100

# seed constants
SEED_LEN_MIN = 5
SEED_LEN_MAX = 20
MIN_ERRORS = 2
MAX_ERRORS = 5

# input file constants
DATA_DIR = "./Freq-data/"

# output file constants
OUTPUT_FILE = "optimal_seeds.out"

"""
""  begin the main program
"""

# compute the optimal seeds for each read one at a time
f_out = open(OUTPUT_FILE, "w")

for i in range(NUM_READS):
    # initialize the data list
    seed_freq = []
    
    # read in the necessary data
    for j in range(SEED_LEN_MIN, SEED_LEN_MAX + 1):
        file_name = DATA_DIR + "seed_" + str(j) + "_" + str(i) + ".dat"
        f = open(file_name, "r")
        
        file_lines = f.read().strip("\n").split("\n")
        seed_freq_tmp = []
        for line in file_lines:
            frequency = line.split("\t")[-1]
            seed_freq_tmp.append(frequency)
        seed_freq.append(seed_freq_tmp)

    # call the optimal seed finder routine
    for errors in range(MIN_ERRORS, MAX_ERRORS + 1):
        optimal_seed_loc = fast_brute.find_optimal_seeds(seed_freq, READ_LEN, SEED_LEN_MIN, SEED_LEN_MAX, errors, 10000)
        
        # print solution to a file!
        f_out.write("Errors: " + str(errors) +  "\n--------\n" + optimal_seed_loc + "--------")
        f_out.flush()
