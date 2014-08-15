#!/usr/bin/gnuplot
#
# AUTHOR: John Emmons

# png
#set terminal pngcairo size 450,362 enhanced font 'Arial,10'
#set output 'seed_freq.png'

#set terminal eps enhanced font "Helvetica,12"
#set output 'seed_freq.eps'

set term epslatex color
set out 'seed_freq.tex'

# color definitions
set border linewidth 1.5

unset key
unset colorbox

# creating the palette (its the same as the above defined one)
#set palette rgb 21,22,23
#set palette rgb 20,0,20
set palette defined (0 'blue', 1 'red')
#set palette model RGB functions 0,1,1
# Axes
set style line 11 lc rgb '#000000' lt 1
set border 1 back ls 11
set xtics nomirror out scale 0.75
unset ytics

set title title_name

set xrange [0:150]
set xtics 0, 10, 100

set yrange [-4.25:.75]
#set cbrange [1:1e10]
#set cbrange[1:*]
#set logscale cb

set cblabel '$\log($occurrences$)$'

set style line 1 lc palette lw 35
set colorbox

set label 'seed length = 5' at 105, .5
set label 'seed length = 6' at 105, 0.25
set label 'seed length = 7' at 105, 0
set label 'seed length = 8' at 105, -.25
set label 'seed length = 9' at 105, -.5
set label 'seed length = 10' at 105, -0.75
set label 'seed length = 11' at 105, -1
set label 'seed length = 12' at 105, -1.25
set label 'seed length = 13' at 105, -1.5
set label 'seed length = 14' at 105, -1.75
set label 'seed length = 15' at 105, -2
set label 'seed length = 16' at 105, -2.25
set label 'seed length = 17' at 105, -2.5
set label 'seed length = 18' at 105, -2.75
set label 'seed length = 19' at 105, -3
set label 'seed length = 20' at 105, -3.25

set label 'seed length = 11 (chunk)' at 105, -3.5
set label 'seed length = 12 (chunk)' at 105, -3.75
set label 'seed length = 13 (chunk)' at 105, -4

#set label '$\vert$' at -0.5,0.5 front

plot 	 data_dir . '/seed_5_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/seed_6_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/seed_7_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/seed_8_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/seed_9_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/seed_10_' . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/seed_11_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/seed_12_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/seed_13_' . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/seed_14_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/seed_15_' . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/seed_16_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/seed_17_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/seed_18_' . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/seed_19_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/seed_20_' . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/seed_5_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/seed_6_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/seed_7_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/seed_8_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/seed_9_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/seed_10_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/seed_11_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/seed_12_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/seed_13_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/seed_14_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/seed_15_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/seed_16_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/seed_17_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/seed_18_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/seed_19_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/seed_20_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/chunk-seed_11_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/chunk-seed_12_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/chunk-seed_13_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black"