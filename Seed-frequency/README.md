Seed Frequency and Optimal Seed Finding Code
======

**Author:** John Emmons

**Date:** August 15, 2014x

**Description:** This was primarily used to create the "heat map" plots used during the summer of 2014. This directory include the code necessary to sample a genome for the number of locations a K-mer occurs and the plotting code to make the heat map. Look under setup and usage for basic instructions on how to use this code. Contact John Emmons @ emmons.john@gmail.com with questions.

**Setup:**

In order to run the plotting code in this directory you will need to configure the files to the type of plot want to produce. Use the "configure" python script to organize and compile the files.

```
   >$ ./configure
```
OR
```
   >$ python configure
```
If you are setting this code up for the first time, you will need to generate hash table in the Bin directory. To do this, simply copy any .fasta file into the Bin directory and run the hash_table_generator.py script.
```
    >$ python2.7 hash_table_generator.py <input_genome.fasta>
```

**Usage:** To generate the seed frequency data (stored in Freq-data) and to create plots, run the command

```
./hotspot_finder.py
```

This script will EXPECT AND INPUT CALLED "reads.inp". This can be modified in the first few lines of the hotspot_finder.py script though.

Any plots generated will be stored in Plots.