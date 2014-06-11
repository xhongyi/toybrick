#!/usr/bin/python
import os

MAX_PREFETCH_HEAD_START = 1000
RUNS_PER_TEST = 3

INPUT_READ_FILE = 'check0'
OUTPUT_BENCHMARK_FILE = 'constant_prefetch.dat'

time_to_completion = [[0 for j in range(RUNS_PER_TEST)  ] for i in range(0, MAX_PREFETCH_HEAD_START) ]


for trial in range(RUNS_PER_TEST):
    for prefetch_head_start in range(1, MAX_PREFETCH_HEAD_START, 1):
        
        out = os.popen('./countPassFilter 3 ' + str(prefetch_head_start) + " < " + INPUT_READ_FILE).read()
        time_to_completion[prefetch_head_start][trial] += float(out.split("\n")[2].split(" ")[-1])
        print prefetch_head_start, time_to_completion[prefetch_head_start][trial]
        
# open the file and write out the collected data
f = open(OUTPUT_BENCHMARK_FILE, "w")

for prefetch_head_start in range(1, MAX_PREFETCH_HEAD_START, 1):
    for trial in range(RUNS_PER_TEST):
        f.write( str(prefetch_head_start) + "\t" + str(time_to_completion[prefetch_head_start][trial]) + "\n")

f.flush()
f.close()
