#!/usr/bin/python
import os, sys

if(len(sys.argv) != 3):
    print "invalid CLI arguments"
    exit()

command_to_run = "./countPassFilter" + " " + str(sys.argv[1]) + " " + str(sys.argv[2]) + " " + "< check0"

print command_to_run
os.system(command_to_run)
