Plotting-script-bitvec
======================

**Author: John Emmons**

**Date: August 13, 2014**

**Description:** The directories,

- time_results
- new_results2
- fp_result

All contain plotting code for creating plots used in the SHD paper. In all directories, you can compile the figures by simples running,

    make

Once make is run, the plots in 'time_results' and 'new_results2' will be copied into a Plots directory. These plots require a little more tender loving care to reach publication quality. From 'time_results' and 'new_results2' you should, 

    cd Plots
    make
    ./bw.sh # Will convert the plot to grayscale (only in time_result)

 
The output plots for 'time_results' and 'new_results2' will be call **final.pdf**. For the plots in 'fp_result', the plots will be call **varyEdits.pdf** and **varySRS.pdf** respectively.