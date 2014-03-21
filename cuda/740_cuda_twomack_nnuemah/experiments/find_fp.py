#!/bin/python

import sys
import os
import subprocess

bash_command = 'python test_gpu_shift.py 1 1 8 3 3 3 3'


for _ in range(1000):
	print _
	process = subprocess.Popen(bash_command.split(), stdout=subprocess.PIPE)

	output = process.communicate()[0]

	output = output.strip()
	lines = output.split('\n')

	if(float(lines[-1].split(' ')[-1]) != 0.0 or float(lines[-2].split(' ')[-1]) != 0.0):
		print '\n'.join(lines)
		exit(0) 

