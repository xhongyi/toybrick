#!/usr/bin/python
import os
import re
import sys

import prefetch_run as pf

"""
Define the runtime constants
"""
MAX_PREFETCH_HEAD_START = 5000
STRIDE = 10
RUNS_PER_TEST = 1

INPUT_READ_FILE = 'check0'
OUTPUT_BENCHMARK_FILE = 'constant_prefetch.dat'

"""
List to the store the results during the loop
"""
time_to_completion = [[[0 for i in range(13)] for j in range(RUNS_PER_TEST)  ] for i in range(0, MAX_PREFETCH_HEAD_START, STRIDE) ]

"""
Print the header information to the intermediate data file
"""
fout = open("safety.out", "w")
fout.write("pref dis" + "\t" + "time(s)" + "\t" + "L1-dcache-loads" + "\t" + "L1-dcache-stores" + "\t" + "L1-dcache-load-stores" + "\t" + "L1-dcache-load-misses" + "\t" + "L1-dcache-store-misses" + "\t" + "LLC-loads" + "\t" + "LLC-stores" + "\t" + "LLC-load-stores" + "\t" + "LLC-load-misses" + "\t" + "LLC-store-misses" + "\t" + "LLC-prefetches" + "\t" + "LLC-prefetch-misses" + "\n")

for trial in range(RUNS_PER_TEST):
    for prefetch_head_start in range(0, MAX_PREFETCH_HEAD_START, STRIDE):
        # compute the current index
        curr_index = prefetch_head_start / STRIDE
        
        """
        Run test and parse results
        """
        out, perf_data = pf.run(prefetch_head_start)
        time, perf_results = pf.parse(out, perf_data)
    
        time_to_completion[curr_index][trial][0] = time
        time_to_completion[curr_index][trial][1:] = perf_results

        """
        Output the results to the console 
        """
        sys.stdout.write(str(prefetch_head_start) + "\t")
        for i in range(len(time_to_completion[curr_index][trial]) - 1):
            sys.stdout.write(str(time_to_completion[curr_index][trial][i]) + "\t")
        sys.stdout.write(str(time_to_completion[curr_index][trial][len(time_to_completion[curr_index][trial]) - 1]) + "\n")
    
        """
        Output the results to a file in case of error during subsequent loops
        """
        fout.write(str(prefetch_head_start) + "\t")
        for i in range(len(time_to_completion[curr_index][trial]) - 1):
            fout.write(str(time_to_completion[curr_index][trial][i]) + "\t")
        fout.write(str(time_to_completion[curr_index][trial][len(time_to_completion[curr_index][trial]) - 1]) + "\n")
        fout.flush()


"""
open the file and write out the collected data
"""
f = open(OUTPUT_BENCHMARK_FILE, "w")

"""
Write the file header
"""
f.write("pref dis" + "\t" + "time(s)" + "\t" + "L1-dcache-loads" + "\t" + "L1-dcache-stores" + "\t" + "L1-dcache-load-stores" + "\t" + "L1-dcache-load-misses" + "\t" + "L1-dcache-store-misses" + "\t" + "LLC-loads" + "\t" + "LLC-stores" + "\t" + "LLC-load-stores" + "\t" + "LLC-load-misses" + "\t" + "LLC-store-misses" + "\t" + "LLC-prefetches" + "\t" + "LLC-prefetch-misses" + "\n")

"""
Output the data in the main loop
"""
for prefetch_head_start in range(0, MAX_PREFETCH_HEAD_START, STRIDE):
    for trial in range(RUNS_PER_TEST):
        curr_index = prefetch_head_start / STRIDE

        f.write(str(prefetch_head_start) + "\t")
        for i in range(len(time_to_completion[curr_index][trial]) - 1):
            f.write(str(time_to_completion[curr_index][trial][i]) + "\t")
        f.write(str(time_to_completion[curr_index][trial][len(time_to_completion[curr_index][trial]) - 1]) + "\n")

f.flush()
f.close()
fout.close()
