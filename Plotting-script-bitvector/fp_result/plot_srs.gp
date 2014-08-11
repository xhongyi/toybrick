set term pslatex color 
set out 'spectrum_350_10_20_10_highin.tex'
set ylabel '\Large False Positive Rate (\%)' offset 0,0.0
set xlabel '\Large SRS Threshold' offset 0.0,-1
#set format y '%6.2f'
set border 31 lw 2
set mytics 5
set yr [0:50]
set xr [1:15]
set xtics 3
set mxtics 3
#set format x '%4.2f'
set nolabel
#set label '$4 \times 10^{14}\,$W/cm$^2$' at 1200,0.1
#set title '\textbf{2-36-2~~~~~$\mathbf{4.0 \times 10^{14}\,}$W/cm$^2$~~~9.5 eV}' 
set key top right spacing 2.2
plot 'Modified-data/srs_threshold.dat' u 1:(100*$2/$3) w lp ps 3 pt 6 lt rgb "#000000" lw 10 t '' 