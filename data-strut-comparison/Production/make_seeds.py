#!/usr/bin/python
import os
import sys
import random

SEED_FILE = "seed.inp"

ALPHABET = ['A', 'C', 'G', 'T']

if(len(sys.argv) != 3):
    print "usage: ./" + sys.argv[0] + " <seed length> <num seeds>\n"
    exit(1)

SEED_LEN = int(sys.argv[1])
NUM_SEEDS = int(sys.argv[2])

seed_out = open(SEED_FILE, 'w')
for i in range(NUM_SEEDS):
    seed = ''
    for j in range(SEED_LEN):
        seed += ALPHABET[random.randint(0,3)]
    seed_out.write(seed + '\n')
