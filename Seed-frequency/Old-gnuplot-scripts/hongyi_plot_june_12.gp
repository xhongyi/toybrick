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

set xrange [0:130]
set xtics 0, 10, 100

set xlabel "Seed Starting Index in Read"

set yrange [-6:-1.25]
#set cbrange [1:1e10]
#set cbrange[1:*]
#set logscale cb

set cblabel 'log(locations)'

set style line 1 lc palette lw 20
set colorbox

#set label '\tiny Length 5' at 105, .5
#set label '\tiny Length 6' at 105, 0.25
#set label '\tiny Length 7' at 105, 0
#set label '\tiny Length 8' at 105, -.25
#set label '\tiny Length 9' at 105, -.5
#set label '\tiny Length 10' at 105, -0.75
#set label '\tiny Length 11' at 105, -1
#set label '\tiny Length 12' at 105, -1.25
#set label '\tiny Length 13' at 105, -1.5
#set label '\tiny Length 14' at 105, -1.75
#set label '\tiny Length 15' at 105, -2
#set label '\tiny Length 16' at 105, -2.25
#set label '\tiny Length 17' at 105, -2.5
#set label '\tiny Length 18' at 105, -2.75
#set label '\tiny Length 19' at 105, -3
#set label '\tiny Length 20' at 105, -3.25

set label '\tiny mrFast 11: 76653  locations' at 105, -1.25
set label '\tiny mrFast 12: 45487 locations' at 105, -1.5
set label '\tiny mrFast 13: 28483 locations' at 105, -1.75
set label '\tiny mrFast 12: 75 locations' at 105, -2
set label '\tiny mrFast 12: 166 locations' at 105, -2.25
set label '\tiny mrFast 12: 869 locations' at 105, -2.5
set label '\tiny Hobbes 20: 4  locations' at 105, -2.75
set label '\tiny Hobbes 20: 38 locations' at 105, -3
set label '\tiny Hobbes 20: 811 locations' at 105, -3.25
set label '\tiny Hobbes 19: 4  locations' at 105, -3.5
set label '\tiny Hobbes 19: 41 locations' at 105, -3.75
set label '\tiny Hobbes 19: 725 locations' at 105, -4
set label '\tiny Hobbes 12: 48 locations' at 105, -4.25
set label '\tiny Hobbes 12: 143 locations' at 105, -4.5
set label '\tiny Hobbes 12: 587 locations' at 105, -4.75
set label '\tiny Optimal 3: 5 locations' at 105, -5.0
set label '\tiny Optimal 4: 19 locations' at 105, -5.25
set label '\tiny Optimal 5: 142 locations' at 105, -5.5

     	 #data_dir . '/seed_5_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 #data_dir . '/seed_6_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 #data_dir . '/seed_7_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 #data_dir . '/seed_8_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 #data_dir . '/seed_9_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 #data_dir . '/seed_10_' . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 #data_dir . '/seed_11_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 #data_dir . '/seed_12_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 #data_dir . '/seed_13_' . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 #data_dir . '/seed_14_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 #data_dir . '/seed_15_' . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 #data_dir . '/seed_16_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 #data_dir . '/seed_17_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 #data_dir . '/seed_18_' . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 #data_dir . '/seed_19_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 #data_dir . '/seed_20_' . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	
	 plot  data_dir . '/chunk-seed_11_' . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/chunk-seed_12_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/chunk-seed_13_' . u_num . '.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/chunk-seed_12_0_31.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/chunk-seed_12_0_32.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/chunk-seed_12_0_33.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
 	 data_dir . '/chunk-seed_12_0_41.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/chunk-seed_12_0_42.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/chunk-seed_12_0_43.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/chunk-seed_12_0_44.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
  	 data_dir . '/chunk-seed_12_0_51.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/chunk-seed_12_0_52.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/chunk-seed_12_0_53.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/chunk-seed_12_0_54.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/chunk-seed_12_0_55.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/optimal_3_1.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/optimal_3_2.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/optimal_3_3.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
 	 data_dir . '/optimal_4_1.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/optimal_4_2.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/optimal_4_3.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/optimal_4_4.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
  	 data_dir . '/optimal_5_1.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/optimal_5_2.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/optimal_5_3.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/optimal_5_4.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/optimal_5_5.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/20hobbes_3_1.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/20hobbes_3_2.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/20hobbes_3_3.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
 	 data_dir . '/20hobbes_4_1.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/20hobbes_4_2.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/20hobbes_4_3.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/20hobbes_4_4.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
  	 data_dir . '/20hobbes_5_1.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/20hobbes_5_2.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/20hobbes_5_3.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/20hobbes_5_4.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/20hobbes_5_5.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/19hobbes_3_1.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/19hobbes_3_2.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/19hobbes_3_3.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
 	 data_dir . '/19hobbes_4_1.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/19hobbes_4_2.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/19hobbes_4_3.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/19hobbes_4_4.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
  	 data_dir . '/19hobbes_5_1.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/19hobbes_5_2.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/19hobbes_5_3.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/19hobbes_5_4.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/19hobbes_5_5.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/13hobbes_3_1.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/13hobbes_3_2.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/13hobbes_3_3.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
 	 data_dir . '/13hobbes_4_1.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/13hobbes_4_2.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/13hobbes_4_3.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/13hobbes_4_4.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
  	 data_dir . '/13hobbes_5_1.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/13hobbes_5_2.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/13hobbes_5_3.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/13hobbes_5_4.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/13hobbes_5_5.dat'  u 1:2:(log($3 + 1)) w l ls 1, \
	 data_dir . '/chunk-seed_11_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/chunk-seed_12_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/chunk-seed_13_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/chunk-seed_12_0_31.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/chunk-seed_12_0_32.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/chunk-seed_12_0_33.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/chunk-seed_12_0_41.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/chunk-seed_12_0_42.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/chunk-seed_12_0_43.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/chunk-seed_12_0_44.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/chunk-seed_12_0_51.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/chunk-seed_12_0_52.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/chunk-seed_12_0_53.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/chunk-seed_12_0_54.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/chunk-seed_12_0_55.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/optimal_3_1.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/optimal_3_2.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/optimal_3_3.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/optimal_4_1.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/optimal_4_2.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/optimal_4_3.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/optimal_4_4.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/optimal_5_1.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/optimal_5_2.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/optimal_5_3.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/optimal_5_4.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/optimal_5_5.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
 	 data_dir . '/20hobbes_3_1.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/20hobbes_3_2.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/20hobbes_3_3.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/20hobbes_4_1.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/20hobbes_4_2.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/20hobbes_4_3.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/20hobbes_4_4.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/20hobbes_5_1.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/20hobbes_5_2.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/20hobbes_5_3.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/20hobbes_5_4.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/20hobbes_5_5.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
 	 data_dir . '/19hobbes_3_1.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/19hobbes_3_2.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/19hobbes_3_3.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/19hobbes_4_1.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/19hobbes_4_2.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/19hobbes_4_3.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/19hobbes_4_4.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/19hobbes_5_1.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/19hobbes_5_2.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/19hobbes_5_3.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/19hobbes_5_4.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/19hobbes_5_5.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/13hobbes_3_1.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/13hobbes_3_2.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/13hobbes_3_3.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/13hobbes_4_1.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/13hobbes_4_2.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/13hobbes_4_3.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/13hobbes_4_4.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/13hobbes_5_1.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/13hobbes_5_2.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/13hobbes_5_3.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/13hobbes_5_4.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 data_dir . '/13hobbes_5_5.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black"
	 
	 #data_dir . '/seed_5_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 #data_dir . '/seed_6_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 #data_dir . '/seed_7_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 #data_dir . '/seed_8_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 #data_dir . '/seed_9_'  . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 #data_dir . '/seed_10_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 #data_dir . '/seed_11_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 #data_dir . '/seed_12_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 #data_dir . '/seed_13_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 #data_dir . '/seed_14_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 #data_dir . '/seed_15_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 #data_dir . '/seed_16_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 #data_dir . '/seed_17_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 #data_dir . '/seed_18_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 #data_dir . '/seed_19_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
	 #data_dir . '/seed_20_' . u_num . '.dat'  u 1:2:(log($3 + 1)) pt 1 ps 1  linecolor rgb "black", \
