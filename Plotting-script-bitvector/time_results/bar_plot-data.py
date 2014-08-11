#!/usr/bin/python
import sys
import numpy as np
import matplotlib as mpl
mpl.use('pgf')

# parse CLI inputs
OUTPUT_FILE = sys.argv[1]
INPUT_FILE = sys.argv[2]
NUM_EDITS = sys.argv[3]

def figsize(scale):
    fig_width_pt = 469.755                          # Get this from LaTeX using \the\textwidth
    inches_per_pt = 1.0/72.27                       # Convert pt to inch
    golden_mean = .35 #(np.sqrt(5.0)-1.0)/2.0            # Aesthetic ratio (you could change this)
    fig_width = fig_width_pt*inches_per_pt*scale    # width in inches
    fig_height = fig_width*golden_mean              # height in inches
    fig_size = [fig_width,fig_height]
    return fig_size


def savefig(filename):
    plt.savefig('{}.pgf'.format(filename))
    plt.savefig('{}.pdf'.format(filename))

pgf_with_latex = {                      # setup matplotlib to use latex for output
    "pgf.texsystem": "pdflatex",        # change this if using xetex or lautex
    "text.usetex": True,                # use LaTeX to write all text
    "font.family": "serif",
    "font.serif": [],                   # blank entries should cause plots to inherit fonts from the document
    "font.sans-serif": [],
    "font.monospace": [],
    "axes.labelsize": 10,               # LaTeX default is 10pt font.
    "text.fontsize": 10,
    "legend.fontsize": 8,               # Make the legend/label fonts a little smaller
    "xtick.labelsize": 8,
    "ytick.labelsize": 8,
    "figure.figsize": figsize(0.9),     # default fig size of 0.9 textwidth
    "pgf.preamble": [
        r"\usepackage[utf8x]{inputenc}",    # use utf8 fonts becasue your computer can handle it :)
        r"\usepackage[T1]{fontenc}",        # plots will be generated using this preamble
        ]
    }
mpl.rcParams.update(pgf_with_latex)

import matplotlib.pyplot as plt

def newfig(width):
    plt.clf()
    fig = plt.figure(figsize=figsize(width))
    ax = fig.add_subplot(111)
    return fig, ax

print INPUT_FILE
f = open(INPUT_FILE, 'r')
f_data = f.read().strip('\n').split('\n')[1:]

shd_data = []
seqan_data = []
swps_data = []
for line in f_data:
    times = line.split('\t')
    shd_data.append(float(times[1]))
    seqan_data.append(float(times[0]))
    swps_data.append(float(times[2]))

f_outing = open(str(NUM_EDITS) + "_speedup.out", "w")
shd_ave = sum(shd_data)/len(shd_data)
seqan_ave = sum(seqan_data)/len(seqan_data)
swps_ave = sum(swps_data)/len(swps_data)

f_outing.write('ave:\t' + str(shd_ave) + '\t' + str(seqan_ave) + '\t' + str(swps_ave) + '\n')
f_outing.write('spu:\t' + str(shd_ave/shd_ave) + '\t' + str(seqan_ave/shd_ave) + '\t' + str(swps_ave/shd_ave) + '\n')
f_outing.write('%su:\t' + str(100*(shd_ave - shd_ave)/shd_ave) + '\t' + str(100*(seqan_ave - shd_ave)/shd_ave) + '\t' + str(100*(swps_ave - shd_ave)/shd_ave) + '\n')

f_outing.close()

N = len(shd_data)
ind = np.arange(N)  # the x locations for the groups
width = .25      # the width of the bars

fig, ax = newfig(1.0)
rects1 = ax.bar(ind, shd_data, width, color='black')
rects2 = ax.bar(ind+width, seqan_data, width, color='r', hatch='/')
rects3 = ax.bar(ind+2*width, swps_data, width, color='y', hatch='.')

# add some
ax.set_ylabel('Time (sec)')
ax.set_xlabel('Benchmarks')
ymax = 10000*(int(max(swps_data) / 10000) + 1)
#ax.set_ylim(0,)
ax.set_yticks( (0, ymax/2, ymax) )

plt.xlim([-width,width*(4*len(shd_data) + 7)])
if(int(NUM_EDITS) == 1):
    ax.set_title(NUM_EDITS + " Edit Tolerated Between Read and Reference")
else:
    ax.set_title(NUM_EDITS + " Edits Tolerated Between Read and Reference")
ax.set_xticks(ind+3*width)
fig.subplots_adjust(bottom=0.275, top=0.9, right=.95)
plt.xticks(rotation=-25)
ax.set_xticklabels( ('ERR240726', 'ERR240727', 'ERR240728', 'ERR240729', 'ERR240730', 'ERR240731', 'ERR240732', 'ERR240733', 'ERR240734', 'ERR240735') )

ax.legend( (rects1[0], rects2[0], rects3[0]), ('SHD', 'Seqan', 'Swps'), loc=1)

def autolabel(rects):
    # attach some text labels
    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x()+rect.get_width()/2., 1.05*height, '%d'%int(height),ha='center', va='bottom')

#autolabel(rects1)
#autolabel(rects2)
#autolabel(rects3)

savefig(OUTPUT_FILE)
