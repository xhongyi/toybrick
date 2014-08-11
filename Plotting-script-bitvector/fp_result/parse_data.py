#!/usr/bin/python
import sys

MIN_ARGS = 5
INPUT_FILE_OFFSET = 4
AF_FILE_OFFSET = 99
AF_FILE_DATA_LINE = 92

CLI_NUM_ERRORS = 1
CLI_OUTPUT = 2
CLI_NUM_FILES =3

if(len(sys.argv) < MIN_ARGS):
    print("""
USAGE: 
./parse_data <num errors>  <output> <num files (n)> <*_results 1> ... <*_results n> <*_swps 1> ... <*_swps n> <*.fastq_AF 1> ... <*.fastq_AF n>
""")

# open output and print header
f_out = open(sys.argv[CLI_OUTPUT], 'w')
f_out.write("SEQAN" + '\t' + "SHD" + '\t' + "SWP" + '\t' + "AF" + '\n')

"""
Read the input files
"""
num_files = int(sys.argv[CLI_NUM_FILES])
for i in range(num_files):
    f1_in = open(sys.argv[i + INPUT_FILE_OFFSET], 'r')
    f2_in = open(sys.argv[i + (INPUT_FILE_OFFSET + num_files)], 'r')
    f3_in = open(sys.argv[i + (INPUT_FILE_OFFSET + 2*num_files)], 'r')
    
    f1_data = f1_in.read().strip('\n').split('\n')
    f2_data = f2_in.read().strip('\n')
    f3_data = f3_in.read().split('\n')[AF_FILE_DATA_LINE + int(sys.argv[CLI_NUM_ERRORS]) * AF_FILE_OFFSET].split('\t')[0].split(' ')[-1]

    # write data to output file
    for data_entry in f1_data:
        raw_data = data_entry.split(' ')[-1]
        f_out.write(raw_data + '\t')

    raw_data = f2_data.split(' ')[-1]
    f_out.write(raw_data + '\t')

    f_out.write(f3_data + '\n')

f_out.close()
