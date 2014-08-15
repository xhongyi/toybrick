Hash Table Creation Scripts
=====

This directory contains the code and information needed to create the hash table files used in the "optimal" seeds code.

**Description:** There are two version of the code "Chunk" and "Continuous". The source code differs and each version can be found in the "Chunk-plots" and "Continuous-plots" directories. 

The continuous version of the code is likely the most useful; it find the number of locatation mapping to a K-mer at every basepair in a read. 

The chunk version, on the other hand, will get the number of location of K-mers at K basepair intervals. To illustrate the differences where the different code version sample (S) and do not sample (X) for a 3-mer

**Read**      : A G T A G A T A G A A G A G ...
**continuous**: S S S S S S S S S S S S S S ...
**chunk**     : S X X S X X S X X S X X S X ...

**Usage:** Rather than compiling this code directly, it is recommended that you use the configuration script in Seed-frequency to compile. From here just,

```
cd ..
./configure # select the options you want
```