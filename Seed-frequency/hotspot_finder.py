#!/usr/bin/python
import subprocess, os

"""
This is determine the shortest and longest kmer 
to be generated, NOT NECCESSARILY WHAT WILL BE 
PLOTTED. Edit the seed_freq.gp file to change 
the plots.
"""
SEED_LEN_MIN = 5
SEED_LEN_MAX = 20

# read hotspot constant
BIN_LOC = "Bin/"
READS_FILE = "reads.inp"
REF_GENOME_NAME = BIN_LOC + "human_g1k_v37.fasta"

#
# find the hotspots for each read in READ_FILE
#
reads = open(READS_FILE, 'r+')
unique_num = 0
for line in reads:
	for i in range(SEED_LEN_MIN,SEED_LEN_MAX + 1):
		print "Creating plot for read numnber:", i, "in the file:", READS_FILE
		
		"""
		Run the hash table code to find out the number of locations for K-mers of length K = i
		"""
		p = subprocess.Popen([BIN_LOC + "read_hotspots", REF_GENOME_NAME + ".ht." + str(i), line.strip("\n")], stdout=subprocess.PIPE)
		out = p.communicate()
				
		"""
		Create the sample file for this read and output the locations information
		"""
		output = open("seed_" + str(i) + "_"  + str(unique_num) + ".dat", 'w')
		
		line_list = out[0].strip("\t").split("\t")
		for j in range(len(line_list)):
			output.write(str(j) + "\t" + str(1.0 - .25 * (i - 3)) + "\t" + str(float(line_list[j]) + 1) + "\n") 

		"""
		Flush and close the file
		"""
		output.flush()
		output.close()

		"""
		Move the sample file for this read
		"""
		p = subprocess.Popen(["mv", "seed_" + str(i) + "_"  + str(unique_num) + ".dat", "Freq-data"], stdout=subprocess.PIPE)
		out = p.communicate()


	"""
	Run the gnuplot file to create the plot for each read
	"""
	os.system("cp" + " " +  "c_seed_freq.tex" + " " + str(unique_num) + "c_seed_freq.tex")

	compile_gnuplot = "gnuplot" + " " + "-e" + " " + '''"data_dir='Freq-data'"''' + " " + "-e" + " " + '''"u_num=''' + "'" + str(unique_num)  + "'" +'"' + " " + "-e" + " " + '''"title_name=''' + "'" + line[:50] + "\n" + line[51:]  + "'" + '"' + " "  + "seed_freq.gp"

	os.system(compile_gnuplot)
	os.system("pdflatex" + " " + str(unique_num) + "c_seed_freq.tex")
	os.system("pdfcrop" + " " + str(unique_num) + "c_seed_freq.pdf")

	os.system("mv" + " " + str(unique_num) + "c_seed_freq-crop.pdf" + " " + "Plots") 

	# remove the autogenerated files
	os.system("rm -rf *.aux *.log *.eps *.pdf")
	os.system("rm -rf" + " " + str(unique_num) + "c_seed_freq.tex")
	os.system("rm -rf" + " " + "seed_freq.tex") 
	unique_num += 1