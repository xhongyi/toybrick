#!/usr/bin/env python
from __future__ import print_function
import random
import sys


######################
## HELPER FUNCTIONS ##
######################

# FUNCTION: insertion
#   INPUTS: read - the original read
#           index - the index at which the insertion will occur
#           base - the base (A, C, G, T) to be inserted
#   OUTPUTS: return_str - the read with the insertion of the base at the index
def insertion(read, index, base):
    return_str = read[0:index]+str(base)+read[index:len(read)-1]
    return return_str

# FUNCTION: deletion
#   INPUTS: read - the original read
#           index - the index at which the deletion will occur
#           base - the base (A, C, G, T) to be inserted at the end of the read
#   OUTPUTS: return_str - the read with the deletion at the index
def deletion(read, index, base):
    return_str = read[0:index]+read[index+1:len(read)]+str(base)
    return return_str

# FUNCTION: replace
#   INPUTS: read - the original read
#           index - the index at which the replacement will occur
#           base - the replacement base (A, C, G, T)
#   OUTPUTS: return_str - the read with a replacement at the index
def replace(read, index, base):
    while(base == read[index]):
        base = random.choice(bases)
    return_str = read[0:index]+str(base)+read[index+1:len(read)]
    return return_str


######################################################################
## MAIN SCRIPT
######################################################################

# get command line arguments
good_file_name = sys.argv[1] #file containing read without edits
edit_file_name = sys.argv[2] #file containing reads with edits
#number_edits = sys.argv[3]
num_lines = int(sys.argv[3]) #number of reads in file
line_length = int(sys.argv[4]) #read length
edits = int(sys.argv[5]) #number of actual edits between reads

# arrays containing bases
bases = ['A','C','T','G']
types_of_edits = ['0','0','0','0','1','1','1','1'] #this can be used to control freq of edit types

# open files
file1 = open(good_file_name,"w+") #original reads
file2 = open(edit_file_name,"w+") #reads with edits
#file3 = open(number_edits,"w+")

# repeat for the number of reads you want
for i in xrange(num_lines):

    # generate a random read for the original reads
    out_str = ""
    for i in xrange(line_length):
        out_str += random.choice(bases)
    print(out_str,file = file1)

    # generate a read with a set number of edits
    edited_string = out_str #start with original read
    indeces_chosen = [] #keep track of the indices at which edits have been made to limit the overlap of edits

    #repeat for the number of edits
    for e in xrange(edits):
        rand_index = random.randint(0,line_length-1) #choose index

        while(rand_index in indeces_chosen): #if that index has already been used, choose another
            rand_index = random.randint(0,line_length-1)
        indeces_chosen.append(rand_index)

        edit_type = random.randint(0,2) #select a type of edit

        # you can use these to hardcode the type of edit
        #edit_type = int(random.choice(types_of_edits))
        #edit_type = 2

        # pick an edit type and edit the read
        if(edit_type == 0): #insertion
            edited_string = insertion(edited_string, rand_index, random.choice(bases))
        elif(edit_type == 1): #deletion
            edited_string = deletion(edited_string, rand_index, random.choice(bases))
        else: #replacement
            edited_string = replace(edited_string, rand_index, random.choice(bases))

    print(edited_string, file = file2) #write the edited read to the other file
    #print(str(edits), file = file3)

file1.close()
file2.close()
