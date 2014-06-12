#!/usr/bin/python

import random

f = open("random_num.dat", "w")

for i in range(1000001):
    f.write(str(int(1000000*random.random())) + "\n")


