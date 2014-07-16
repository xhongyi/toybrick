#!/usr/bin/python

import random, os

REPEAT = 1000
OUT_FILE = "random_num.inp"

f = open(OUT_FILE, "w")

rand_num = []

for i in range(REPEAT):
    rand_num.append(int(1000000*random.random()))

for i in range(1000001):
#    f.write("123456\n")
#   f.write(str(rand_num[int(REPEAT * random.random())]) + "\n")
#    f.write(str(rand_num[i % REPEAT]) + "\n")
    f.write(str(int(1000000*random.random())) + "\n")

os.system('cp '  + OUT_FILE + ' ' + 'prefetch_'  + OUT_FILE)
