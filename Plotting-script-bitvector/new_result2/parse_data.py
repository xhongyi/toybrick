#!/usr/bin/python
import sys

MIN_ARGS = 5
INPUT_FILE_OFFSET = 3

if(len(sys.argv) < MIN_ARGS):
    print("""
USAGE: 
./parse_data <output> <num files (n)> <*_results 1> ... <*_results n> <*_swps 1> ... <*_swps n>
""")

# open output and print header
f_out = open(sys.argv[1], 'w')
f_out.write("""bv    af""" + '\n')

num_files = int(sys.argv[2])
for i in range(num_files):
    f1_in = open(sys.argv[i + INPUT_FILE_OFFSET], 'r')
    f2_in = open(sys.argv[i + (INPUT_FILE_OFFSET + num_files)], 'r')
    
    f1_data = f1_in.read().strip('\n').split('\n')
    f2_data = f2_in.read().strip('\n').split('\n')


    my_data1 = []
    my_data2 = []

    # write data to output file
    for j in range(len(f1_data)):
        if(j % 2 != 0):
            my_data2.append(list(f1_data[j].split('\t')))
            my_data1.append(list(f2_data[j].split('\t')))

    total_mappings = sum(map(float, my_data2[0]))

    length = len(my_data2[0])

    bitvec_filter = sum(map(float, my_data2[1][:length-1]))
    af_filter = sum(map(float, my_data1[0][:length-1]))

    # print my_data1, len(my_data1[1][:len(my_data1) - 1])
    #print total_mappings, af_filter, bitvec_filter
    f_out.write(str(100*bitvec_filter/total_mappings) + '\t' + str(100*af_filter/total_mappings) + '\n')

f_out.close()
