SHELL:= /bin/bash

DATA_DIR=Modified-data
PLOTS_DIR=Plots

all: fig1 fig2
	echo "done"

fig1:
	gnuplot plot_srs.gp
	latex srs_helper.tex
	dvips srs_helper.dvi
	ps2pdf srs_helper.ps
	mv srs_helper.pdf varySrs.pdf
	cp varySrs.pdf ../../SHD/sections/figures/
	echo "done with fig 1"

fig2:
	gnuplot plot_edit.gp
	latex edit_helper.tex
	dvips edit_helper.dvi
	ps2pdf edit_helper.ps
	mv edit_helper.pdf varyEdits.pdf
	cp varyEdits.pdf ../../SHD/sections/figures/
	echo "done with fig 2"