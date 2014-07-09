#!/bin/sh
gcc Lookup.c BWT.o dictionary.o DNACount.o HSP.o HSPstatistic.o         iniparser.o inistrlib.o karlin.o MemManager.o MiscUtilities.o QSufSort.o       r250.o Socket.o TextConverter.o 2BWT-Interface.o -lm -o Lookup
