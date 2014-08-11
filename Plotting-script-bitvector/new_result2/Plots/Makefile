
all:
	pdflatex combine_fig.tex
	pdflatex final.tex
	pdfcrop final.pdf
	pdftk final-crop.pdf cat 1Left output final.pdf