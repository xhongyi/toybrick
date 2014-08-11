#!/usr/bin/python
import sys
import numpy as np
import matplotlib as mpl
from matplotlib.patches import Rectangle

mpl.use('pgf')

"""
parse CLI inputs
"""
OUTPUT_FILE = sys.argv[1]
INPUT_FILE = sys.argv[2]
NUM_EDITS = sys.argv[3]

print "Starting work on " + OUTPUT_FILE + ".{pdf,pgf}. This may take 10+ minutes, be patient." 

def figsize(scale):
    fig_width_pt = 469.755                          # Get this from LaTeX using \the\textwidth
    inches_per_pt = 1.0/72.27                       # Convert pt to inch
    golden_mean = .155#(np.sqrt(5.0)-1.0)/2.0            # Aesthetic ratio (you could change this)
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
    "axes.labelsize": 12,               # LaTeX default is 10pt font.
    "text.fontsize": 12,
    "legend.fontsize": 12,               # Make the legend/label fonts a little smaller
    "xtick.labelsize": 12,
    "ytick.labelsize": 12,
    "figure.figsize": figsize(0.9),     # default fig size of 0.9 textwidth
    "pgf.preamble": [
        r"\usepackage[utf8x]{inputenc}",    # use utf8 fonts becasue your computer can handle it :)
        r"\usepackage[T1]{fontenc}",        # plots will be generated using this preamble
        ]
    }
mpl.rcParams.update(pgf_with_latex)
mpl.rcParams.update({'font.size': 12})

import matplotlib.pyplot as plt

"""
used to define a new figure with settingin pgf_with_latex
"""
def newfig(width):
    plt.clf()
    fig = plt.figure(figsize=figsize(width))
    ax = fig.add_subplot(111)
    return fig, ax

"""
this function will label the tops of the bar with their height (near end of code are example calls)
"""
def autolabel(rects):
    # attach some text labels
    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x()+rect.get_width()/2., 1.05*height, '%d'%int(height),ha='center', va='bottom')


"""
open files and read in data
"""
f = open(INPUT_FILE, 'r')
f_data = f.read().strip('\n').split('\n')[1:]

bitvec = []
af = []
for line in f_data:
    times = line.split('\t')
    bitvec.append(float(times[0]))
    af.append(float(times[1]))

""" 
set information for the bar plot and create the rectangles
"""
N = len(bitvec)
ind = np.arange(N)  # the x locations for the groups
width = .33      # the width of the bars

fig, ax = newfig(1.0)
rects1 = ax.bar(ind, bitvec, width, color='black')
rects2 = ax.bar(ind+width, af, width, color='#33CC33', hatch='.')

"""
COMMENTED OUT AXES INFORMATION TO MAKE PLOT MORE COMPACT
"""
'''# xtick mark only on fitfh plot
ax.set_ylabel('Time (sec)')
if(int(NUM_EDITS) == 5): # adds xlabel to the fifth plot
    fig.subplots_adjust(bottom=0.275, top=.95, right=.99, left=.125)
    ax.set_xlabel('Benchmarks')
    plt.xticks(rotation=-25)
    ax.set_xticklabels( ('ERR240726', 'ERR240727', 'ERR240728', 'ERR240729', 'ERR240730', 'ERR240731', 'ERR240732', 'ERR240733', 'ERR240734', 'ERR240735') )
else:
    fig.subplots_adjust(bottom=0.05, top=.95, right=.99, left=.125)
'''


# xtick marks on every plot
plt.xticks(rotation=-45)
ax.set_xticklabels( ('ERR240726_1', 'ERR240726_2', 'ERR240727_1', 'ERR240727_2', 'ERR240728_1', 'ERR240728_2', 'ERR240729_1', 'ERR240729_2', 'ERR240730_1', 'ERR240730_2') )
ax.set_xlabel('Benchmarks')


'''
plt.tick_params(\
    axis='x',          # changes apply to the x-axis
    which='both',      # both major and minor ticks are affected
    bottom='off',      # ticks along the bottom edge are off
    top='off',         # ticks along the top edge are off
    labelbottom='off') # labels along the bottom edge are off
'''

fig.subplots_adjust(bottom=0.95, top=.99, right=.99, left=.125)

'''
if(int(NUM_EDITS) == 1):
    ax.text(.1,.99*ymax, str(NUM_EDITS) + " errors tolerated") 
else:
    ax.text(.1,.90*ymax, str(NUM_EDITS) + " errors tolerated") 
'''

'''
if(int(NUM_EDITS) == 1):
    ax.set_title(NUM_EDITS + " Edit Tolerated Between Read and Reference")
else:
    ax.set_title(NUM_EDITS + " Edits Tolerated Between Read and Reference")
'''

"""
Set the only 3 ytick marks (scale is percentage so out 0%, 25%, and 50%)
"""
ax.set_yticks( (0, 25, 50) )

"""
Set the x lim based on number of rectangles and distance between xtick marks
"""
plt.xlim([-width,width*(3*len(bitvec) + 7)])
ax.set_xticks(ind+3*width)
#ax.get_xaxis().set_visible(False)
ax.get_yaxis().set_visible(False)


"""
Plot the rectangle and set legend title based on number of erros
"""
'''if(int(NUM_EDITS) == 1):
    legend = ax.legend( (rects1[0], rects2[0]), ('SHD', 'AF'), loc=1, title=str(NUM_EDITS) + " Error")
else:
    legend = ax.legend( (rects1[0], rects2[0]), ('SHD', 'AF'), loc=1, title=str(NUM_EDITS) + " Errors")
plt.setp(legend.get_title(),fontsize='12')
'''
#autolabel(rects1)
#autolabel(rects2)
#autolabel(rects3)

"""
output the pgf and pdflatex documents
"""
savefig(OUTPUT_FILE)

print "Done with " + OUTPUT_FILE + ".{pdf,pgf}"
