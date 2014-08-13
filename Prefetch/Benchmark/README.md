Prefetch Benchmarking
=====

**Usage:**

To make the binaries and prepare for testing just run:

    ./make_and_copy.sh

This is will compile the necessary files and copy the binaries to **this** directory for testing.

To configure the testing, you will have to edit the prefetch_benchmark.py file. No other files should need to be edited for basic configuration.

To run the test:

    ./prefetch_benchmark.py

Expects the input file to be called 'check0' to be a simple list of reads separated by newlines. You can always edit the variable INPUT_READ_FILE to change the input file name.

The output files will be in **this** directory and will be called 'constant_prefetch.dat' assuming you do not make any changes to the configuration. You can always go into prefetch_benchmark.py and edit the variable OUTPUT_BENCHMARK_FILE. You s