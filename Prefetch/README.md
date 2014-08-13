Software Prefetching Test
=====

**Author: John Emmons**
**Date: August 13**

**Description:** The code is these directory was created to test the effects of software piplining on the performance H. Xin's bitvector filtering algorithm. Several different tests were carried out, but as of August 13th the code is designed to fetch reads from a file in blocks of 1000. Once read in, X number of reads are prefetched into the cache using mm_prefetch (X is varied in the python testing script). 

Details on how to configure and use the script are provided in the Benchmark Directory.  

Also, the Old directory contains old versions of the code that tested over methods of prefetching. These methods did not appear to give meaningful results and are not recommended for use.