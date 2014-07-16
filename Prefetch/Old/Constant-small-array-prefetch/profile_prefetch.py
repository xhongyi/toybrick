#!/usr/bin/python
import os, re, sys

MAX_PREFETCH_HEAD_START = 100
STRIDE = 1
RUNS_PER_TEST = 5

INPUT_READ_FILE = 'check0'
OUTPUT_BENCHMARK_FILE = 'constant_prefetch.dat'

time_to_completion = [[[0 for i in range(13)] for j in range(RUNS_PER_TEST)  ] for i in range(0, MAX_PREFETCH_HEAD_START, STRIDE) ]

fout = open("safety.out", "w")
fout.write("pref dis" + "\t" + "time(s)" + "\t" + "L1-dcache-loads" + "\t" + "L1-dcache-stores" + "\t" + "L1-dcache-load-stores" + "\t" + "L1-dcache-load-misses" + "\t" + "L1-dcache-store-misses" + "\t" + "LLC-loads" + "\t" + "LLC-stores" + "\t" + "LLC-load-stores" + "\t" + "LLC-load-misses" + "\t" + "LLC-store-misses" + "\t" + "LLC-prefetches" + "\t" + "LLC-prefetch-misses" + "\n")

for trial in range(RUNS_PER_TEST):
    for prefetch_head_start in range(0, MAX_PREFETCH_HEAD_START, STRIDE):
        # compute the current index
        curr_index = prefetch_head_start / STRIDE

        command_to_run = 'perf stat -e L1-dcache-loads:u -e L1-dcache-stores:u -e L1-dcache-load-stores:u -e L1-dcache-load-misses:u -e L1-dcache-store-misses:u -e LLC-loads:u -e LLC-stores:u -e LLC-load-stores:u -e LLC-load-misses:u -e LLC-store-misses:u -e LLC-prefetches:u -e LLC-prefetches-misses:u ./start.py 3 ' +  str(prefetch_headxz_start) + " 2> test_prefetch_size.log"
        print command_to_run
        
        out =  os.popen(command_to_run).read()
        fin = open("test_prefetch_size.log", "r")

        # parse out the cache information
        fin_data = fin.read().split("\n")
        for i in range(len(time_to_completion[curr_index][trial]) - 1):
            time_to_completion[curr_index][trial][i + 1] = [int(s) for s in re.sub(",","",fin_data[4 + 2 * i]).split() if s.isdigit()][0]
            print i
        
        # parse out the time to run the bivector filtering
        time_to_completion[curr_index][trial][0] = float(out.split("\n")[2].split(" ")[-1])

        # print and write to file incase of error
        sys.stdout.write(str(prefetch_head_start) + "\t")
        for i in range(len(time_to_completion[curr_index][trial]) - 1):
            sys.stdout.write(str(time_to_completion[curr_index][trial][i]) + "\t")
        sys.stdout.write(str(time_to_completion[curr_index][trial][len(time_to_completion[curr_index][trial]) - 1]) + "\n")

        fout.write(str(prefetch_head_start) + "\t")
        for i in range(len(time_to_completion[curr_index][trial]) - 1):
            fout.write(str(time_to_completion[curr_index][trial][i]) + "\t")
        fout.write(str(time_to_completion[curr_index][trial][len(time_to_completion[curr_index][trial]) - 1]) + "\n")
        fout.flush()


# open the file and write out the collected data
f = open(OUTPUT_BENCHMARK_FILE, "w")

# write the file header
f.write("pref dis" + "\t" + "time(s)" + "\t" + "L1-dcache-loads" + "\t" + "L1-dcache-stores" + "\t" + "L1-dcache-load-stores" + "\t" + "L1-dcache-load-misses" + "\t" + "L1-dcache-store-misses" + "\t" + "LLC-loads" + "\t" + "LLC-stores" + "\t" + "LLC-load-stores" + "\t" + "LLC-load-misses" + "\t" + "LLC-store-misses" + "\t" + "LLC-prefetches" + "\t" + "LLC-prefetch-misses" + "\n")

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
