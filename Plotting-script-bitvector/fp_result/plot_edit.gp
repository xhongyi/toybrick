set term pslatex color 
set out 'edit.tex'
set ylabel '\Large False Positive Rate (\%)' offset 1,0.0
set xlabel '\Large Error Threshold ($e$)' offset 0.0,-1
#set format y '%6.2f'
set border 31 lw 2
set mytics 5
set yr [0:100]
set xr [0:15]
set xtics 3
set mxtics 3
#set format x '%4.2f'
set nolabel
#set label '$4 \times 10^{14}\,$W/cm$^2$' at 1200,0.1
#set title '\textbf{2-36-2~~~~~$\mathbf{4.0 \times 10^{14}\,}$W/cm$^2$~~~9.5 eV}'

set label '10 \%' at 13.25, 10
 
set key top right spacing 2.2
f(x) = (x < 13) ? 10 : 1/0
plot 'Modified-data/edit_threshold.dat' u 1:(100*$2/$3) w lp ps 3 pt 6 lt rgb "#000000" lw 10 t '', f(x) ls 2 lc 'black' t ''

