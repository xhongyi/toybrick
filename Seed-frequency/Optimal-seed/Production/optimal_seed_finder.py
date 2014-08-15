#!/usr/bin/python
import os, sys
import brute_force_optimization

# read constant
READ_LEN = 100

# seed constants
SEED_LEN_MIN = 10
SEED_LEN_MAX = 20
MIN_ERRORS = 3
MAX_ERRORS = 5

# input file constants
DATA_DIR = "./Freq-data/"

# output file constants
OUTPUT_FILE = "best_seeds.dat"

"""
""  begin the main program
"""

if(len(sys.argv) != 3):
    print "usage: ./optimal_seed_finder <starting read> <number of reads>\n"
    exit()

STARTING_READ = int(sys.argv[-2])
NUM_READS = int(sys.argv[-1])

# compute the optimal seeds for each read one at a time
f_out = open(OUTPUT_FILE, "w")

for i in range(STARTING_READ, STARTING_READ + NUM_READS):
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

#    print "seed freq", len(seed_freq), seed_freq
    
    # call the optimal seed finder routine
    for errors in range(MIN_ERRORS, MAX_ERRORS + 1):
        optimal_seed_loc = brute_force_optimization.find_optimal_seeds(seed_freq, READ_LEN, SEED_LEN_MIN, SEED_LEN_MAX, errors)

        # print solution to a file!
        f_out.write(str(errors) + "\t" + str(optimal_seed_loc[0]) + "\t" + str(optimal_seed_loc[1]) + "\n")
        f_out.flush()
