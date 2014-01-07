#!/usr/bin/env python
from __future__ import print_function
import sys
import os

# read in the arguments
num_tests = int(sys.argv[1]) #the number of tests you want
num_reads = int(sys.argv[2]) #the number of reads per test
num_read_len = int(sys.argv[3]) #the read length
num_edits = int(sys.argv[4]) #the number of edits per read
num_edits_allowed = int(sys.argv[5]) #the edit threshold (e.g., 3 if you only want to allow 3 edits)
num_vicinity = int(sys.argv[6]) #the bit flip vicinity
num_shifts = int(sys.argv[7]) #the maximum shift distance when XORing

# print out test information
print("num_tests: "+str(num_tests))
print("num_reads: "+str(num_reads))
print("num_read_len: "+str(num_read_len))
print("num_edits: "+str(num_edits))
print("num_edits_allowed: "+str(num_edits_allowed))
print("num_vicinity: "+str(num_vicinity))
print("num_vicinity: "+str(num_shifts))

file1_name = "ts_"+str(num_reads)+"_"+str(num_read_len)+".txt"
file2_name = "ts_"+str(num_reads)+"_"+str(num_read_len)+"_edit.txt"
pop_output = "pop_output.txt"

#print("file1_name: "+str(file1_name))
#print("file2_name: "+str(file2_name))
#print("")

# command line strings

#   this command generates the reads
generator_cmd = "./generator.py "+str(file1_name)+" "+str(file2_name)+" "+str(num_reads)+" "+str(num_read_len)+" "+str(num_edits)

#   this command runs the CUDA code
gpu_cmd = "../inj_taylor_shift.out "+str(file1_name)+" "+str(file2_name)+" "+str(num_reads)+" "+str(num_read_len)+" "+str(num_edits_allowed)+" "+str(num_vicinity)+" "+str(num_shifts)


#################################################################
## ACTUAL TEST SCRIPT
#################################################################
total_false_negatives = 0
total_false_positives = 0

# repeat for the number of tests
for i in xrange(num_tests):

    #print("===================================================================")
    #print("===================================================================")
    #print("generator_cmd: "+str(generator_cmd))

    # run the script to generate the reads and run the GPU code
    os.system(generator_cmd)
    #print("gpu_cmd: "+str(gpu_cmd))
    os.system(gpu_cmd)

    # read the output from the GPU code
    fo = open(pop_output, 'r')

    gpu_output = fo.readline()
    gpu_data = gpu_output.split()
    gpu_passing = int(gpu_data[0]) #get the number of passing reads
    gpu_failing = int(gpu_data[1]) #get the number of failing reads
    #print(gpu_data[1])
    #print(gpu_data[0])
    #print("")
    #print("Test:\t"+str(i))
    false_positives = 0
    false_negatives = 0
    if(num_edits <= num_edits_allowed): #false negatives
        total_false_negatives += gpu_failing
    else: #false positives
        total_false_positives += gpu_passing

    fo.close();

#print out information about the tests
print("total false negatives: "+str(total_false_negatives))
print("total false positives: "+str(total_false_positives))
print("total reads: "+str(num_reads*num_tests))
print("false negatives ratio: "+str(float(total_false_negatives)/(float(num_reads)*float(num_tests))))
print("false positives ratio: "+str(float(total_false_positives)/(float(num_reads)*float(num_tests))))
