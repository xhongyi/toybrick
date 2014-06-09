#!/usr/bin/python
import os

RUNS_PER_TEST = 1
INPUT_READ_FILE = 'check0'
OUTPUT_BENCHMARK_FILE = prefetch_updated_results.log

f = open(OUTPUT_BENCHMARK_FILE, "w")

for prefetch_size in range(100, 10000, 100):
    time = 0.0

    for _ in range(RUNS_PER_TEST):
        out = os.popen('./countPassFilter 3 ' + str(prefetch_size) + " < " + INPUT_READ_FILE).read()

        time += float(out.split("\n")[2].split(" ")[-1])

    f.write( str(prefetch_size) + "\t" + str(time / RUNS_PER_TEST) + "\n")
    f.flush()
