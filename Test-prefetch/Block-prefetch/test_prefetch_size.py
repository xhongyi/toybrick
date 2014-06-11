#!/usr/bin/python
import os

PREFETCH_AMT = 100
RUNS_PER_TEST = 5

INPUT_READ_FILE = 'check0'
OUTPUT_BENCHMARK_FILE = 'constant_prefetch.dat'

time_to_completion = [[0 for j in range(RUNS_PER_TEST)  ] for i in range(0, PREFETCH_AMT) ]


for trial in range(RUNS_PER_TEST):
    for prefetch_amt in range(1, PREFETCH_AMT, 1):
        
        out = os.popen('./countPassFilter 3 ' + str(prefetch_amt) + " < " + INPUT_READ_FILE).read()
        time_to_completion[prefetch_amt][trial] += float(out.split("\n")[2].split(" ")[-1])
        print prefetch_amt, time_to_completion[prefetch_amt][trial]
        
# open the file and write out the collected data
f = open(OUTPUT_BENCHMARK_FILE, "w")

for prefetch_amt in range(1, PREFETCH_AMT, 1):
    for trial in range(RUNS_PER_TEST):
        f.write( str(prefetch_amt) + "\t" + str(time_to_completion[prefetch_amt][trial]) + "\n")

f.flush()
f.close()
