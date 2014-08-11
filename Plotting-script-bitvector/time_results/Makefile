SHELL:= /bin/bash

DATA_DIR=Modified-data
PLOTS_DIR=Plots
MAX_EDITS=5
PLOTS_TO_COMPILE=0_errors 1_errors 2_errors \
	 	3_errors 4_errors 5_errors

# extensions are added in commands below
PLOT_FILE=bar_plot

all: compileFigures
	@echo "Done plotting figures. Plots are in the directory:" $(PLOTS_DIR) 

compileFigures: compileData
	errors=0; for plot in $(PLOTS_TO_COMPILE); do \
		./bar_plot.py $$plot ./$(DATA_DIR)/$$plot.dat $$errors; \
		mv $$plot.pdf $(PLOTS_DIR)/$$plot.pdf; \
		mv $$plot.pgf $(PLOTS_DIR)/$$plot.pgf; \
		((errors=errors + 1)); \
	done

compileData: 
	./compile_data.sh $(DATA_DIR) $(MAX_EDITS)

clean:
	for plot in $(PLOTS_TO_COMPILE); do \
		rm -rf $$plot.tex $(DATA_DIR)/$$plot.dat;\
	done
	rm -rf *.pdf *.log *.aux $(PLOT_FILE).dvi
	rm -rf $(PLOTS_DIR)/*.{pdf,pgf}
