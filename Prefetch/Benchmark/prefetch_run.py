#!/usr/bin/python
import os
import re

def run(head_start):
    profiler = 'perf'
    options = 'stat -e L1-dcache-loads:u -e L1-dcache-stores:u -e L1-dcache-load-stores:u -e L1-dcache-load-misses:u -e L1-dcache-store-misses:u -e LLC-loads:u -e LLC-stores:u -e LLC-load-stores:u -e LLC-load-misses:u -e LLC-store-misses:u -e LLC-prefetches:u -e LLC-prefetches-misses:u'
    program = './start.py 3 ' +  str(head_start) + ' 2> perf.log' 

    """
    Concatenate the perf profiler, options, and progam to make runnable bash command
    """
    command2run = profiler + ' ' + options + ' ' + program
    print command2run

    """
    Run command and capture output from stdout and stderr
    """
    out =  os.popen(command2run).read()
    
    perf_output = open("perf.log", "r")
    perf_data = perf_output.read().strip('\n').split('\n')
    print '\n'.join(perf_data)

    return out, perf_data

def parse(out, perf_data):
    """
    Parse the cache results from perf
    """
    perf_results = []
    for i in range(12): # there are 12 options in 'run'
        perf_results.append([int(s) for s in re.sub(",","",perf_data[4 + i]).split() if s.isdigit()][0])

    """
    Parse out the run time from the output
    """
    time = float(out.split("\n")[2].split(" ")[-1])

    return time, perf_results
