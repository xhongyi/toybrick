#!/usr/bin/python
import os, re

MAX_PREFETCH_HEAD_START = 500
STRIDE = 1
RUNS_PER_TEST = 3

INPUT_READ_FILE = 'check0'
OUTPUT_BENCHMARK_FILE = 'constant_prefetch.dat'

time_to_completion = [[[0,0,0,0] for j in range(RUNS_PER_TEST)  ] for i in range(0, MAX_PREFETCH_HEAD_START, STRIDE) ]

fout = open("safety.out", "w")

for trial in range(RUNS_PER_TEST):
    for prefetch_head_start in range(1, MAX_PREFETCH_HEAD_START, STRIDE):
        # compute the current index
        curr_index = prefetch_head_start / STRIDE

        command_to_run = 'perf stat -B -e cache-references,cache-misses ./start.py 3 ' +  str(prefetch_head_start) + " 2> test_prefetch_size.log"
        print command_to_run
        
        out =  os.popen(command_to_run).read()
        fin = open("test_prefetch_size.log", "r")
        
        # parse out the % of cache miss
        fin_data = fin.read().split("\n")
        time_to_completion[curr_index][trial][2] = [int(s) for s in re.sub(",","",fin_data[4]).split() if s.isdigit()][0]
        time_to_completion[curr_index][trial][3] = [int(s) for s in re.sub(",","",fin_data[5]).split() if s.isdigit()][0]
        time_to_completion[curr_index][trial][1] = round(100 * time_to_completion[curr_index][trial][3] / time_to_completion[curr_index][trial][2], 2)


        # parse out the time to run the bivector filtering
        time_to_completion[curr_index][trial][0] = float(out.split("\n")[2].split(" ")[-1])

        # print and write to file incase of error
        print prefetch_head_start, time_to_completion[curr_index][trial][0], time_to_completion[curr_index][trial][1],time_to_completion[curr_index][trial][2],time_to_completion[curr_index][trial][3]
        fout.write(str(prefetch_head_start) + "\t" + str(time_to_completion[curr_index][trial][0]) + "\t" + str(time_to_completion[curr_index][trial][1]) + "\t" + str(time_to_completion[curr_index][trial][2]) + "\t"  + str(time_to_completion[curr_index][trial][3]) + "\n")
        fout.flush()


# open the file and write out the collected data
f = open(OUTPUT_BENCHMARK_FILE, "w")

# write the file header
f.write("pref-hs\ttime\t%-cmiss\tcref\tcmiss\n")

for prefetch_head_start in range(1, MAX_PREFETCH_HEAD_START, STRIDE):
    for trial in range(RUNS_PER_TEST):
        curr_index = prefetch_head_start / STRIDE
        f.write( str(prefetch_head_start) + "\t" + str(time_to_completion[curr_index][trial][0]) + "\t" + str(time_to_completion[curr_index][trial][1]) + "\t" + str(time_to_completion[curr_index][trial][2]) +"\t" + str(time_to_completion[curr_index][trial][3]) + "\n")

f.flush()
f.close()
fout.close()
