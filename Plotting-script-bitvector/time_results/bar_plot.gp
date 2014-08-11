#!/usr/bin/gnuplot

set terminal pslatex color
set output extern_output

set title the_title
set xlabel '\phantom{blank}'
set ylabel 'Time (sec)'

unset xtics
unset key

set xrange[0:7]
set yrange[0:*]

path2data = './' . data_dir . '/' . data_file

mean1(x) = col1mean
mean2(x) = col2mean
mean3(x) = col3mean

fit mean1(x) path2data u 0:2 via col1mean
fit mean2(x) path2data u 0:1 via col2mean
fit mean3(x) path2data u 0:3 via col3mean

set object 1 rect from 1,0 to 2,col1mean fc rgb 'black'
set object 2 rect from 3,0 to 4,col2mean fc rgb '#CC0000'
set object 3 rect from 5,0 to 6,col3mean fc rgb '#000099'

labelPos = -col3mean/20

set label center "SHD" at 1.5,labelPos
set label center "Seqan" at 3.5,labelPos 
set label center "swps" at 5.5,labelPos 

plot mean3(x) + 50 lw 0 lc rgb "#ffffffff"


