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
    golden_mean = .235#(np.sqrt(5.0)-1.0)/2.0            # Aesthetic ratio (you could change this)
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
used to define a new figure with setting pgf_with_latex
"""
def newfig(width):
    plt.clf()
    fig, (ax1, ax2) = plt.subplots(2,1,sharex=True)
    return fig, ax1, ax2

"""
this function will label the tops of the bar with their height (near end of code are example calls)
"""
def autolabel(rects):
    # attach some text labels
    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x()+rect.get_width()/2., 1.05*height, '%d'%int(height),ha='center', va='bottom', weight='bold')


"""
open files and read in data
"""
f = open(INPUT_FILE, 'r')
f_data = f.read().strip('\n').split('\n')[1:]

shd_data = []
seqan_data = []
swps_data = []
af_data = []
for line in f_data:
    times = line.split('\t')
    shd_data.append(float(times[1]))
    seqan_data.append(float(times[0]))
    swps_data.append(float(times[2]))
    af_data.append(float(times[3]))

YMAX = [10000,27500, 60000, 125000, 250000, 500000]
ymax = YMAX[int(NUM_EDITS)]
cutoff=[2500, 5000, 10000, 25000, 50000, 100000] 
PADDING=[500,1000,2000,4000,15000,20000]
CUTOFF = cutoff[int(NUM_EDITS)] + PADDING[int(NUM_EDITS)]

swps_data_helper = [ float(CUTOFF) for i in range(len(swps_data)) ]

""" 
set information for the bar plot and create the rectangles
"""
N = len(shd_data)
ind = np.arange(N)  # the x locations for the groups
width = .20      # the width of the bars

fig, ax, ax1 = newfig(1.0)
rects1 = ax1.bar(ind, shd_data, width, color='black')
rects2 = ax1.bar(ind+width, seqan_data, width, color='r', hatch='///')
rects3 = ax1.bar(ind+2*width, af_data, width, color='#33CC33', hatch='...')
rects4 = ax1.bar(ind+3*width, swps_data_helper, width, color='#6666FF', hatch='---')
rects4 = ax.bar(ind+3*width, swps_data, width, color='#6666FF', hatch='---')

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

'''
# xtick marks on every plot
plt.xticks(rotation=-25)
ax.set_xticklabels( ('ERR240726', 'ERR240727', 'ERR240728', 'ERR240729', 'ERR240730', 'ERR240731', 'ERR240732', 'ERR240733', 'ERR240734', 'ERR240735') )
'''

'''
plt.tick_params(\
    axis='x',          # changes apply to the x-axis
    which='both',      # both major and minor ticks are affected
    bottom='off',      # ticks along the bottom edge are off
    top='off',         # ticks along the top edge are off
    labelbottom='off') # labels along the bottom edge are off
'''

fig.subplots_adjust(bottom=0.06, top=.94, right=.99, left=.125)

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
Set the only 3 ytick marks
"""
padding = PADDING[int(NUM_EDITS)]
ax1.set_ylim(0,CUTOFF)
ax.set_ylim(ymax - CUTOFF, ymax)
ax1.set_yticks( (0, CUTOFF - padding) )
ax.set_yticks( (ymax - CUTOFF + padding, ymax) )

ax.spines['bottom'].set_visible(False)
ax1.spines['top'].set_visible(False)
ax.xaxis.tick_top()
ax.tick_params(labeltop='off') # don't put tick labels at the top
ax1.xaxis.tick_bottom()

d=0.015
# arguments to pass plot, just so we don't keep repeating them
kwargs = dict(transform=ax.transAxes, color='k', clip_on=False)
ax.plot((-d,+d),(-d,+d), **kwargs)      # top-left diagonal
ax.plot((1-d,1+d),(-d,+d), **kwargs)    # top-right diagonal

kwargs.update(transform=ax1.transAxes)  # switch to the bottom axes
ax1.plot((-d,+d),(1-d,1+d), **kwargs)   # bottom-left diagonal
ax1.plot((1-d,1+d),(1-d,1+d), **kwargs) # bottom-right diagonal

"""
Set the x lim based on number of rectangles and distance between xtick marks
"""
plt.xlim([-width,width*(5*len(shd_data))])
ax.set_xticks(ind+3*width)
ax.get_xaxis().set_visible(False)
ax1.get_xaxis().set_visible(False)


"""
Plot the rectangle and set legend title based on number of errors
"""
box = ax.get_position()
ax.set_position([box.x0, box.y0, box.width * 0.775, box.height])
ax1.set_position([box.x0, box.y0 - 0.5, box.width * 0.775, box.height])

if(int(NUM_EDITS) == 1):
    legend = ax.legend( (rects1[0], rects2[0], rects3[0], rects4[0]), ('SHD', 'SeqAn', 'AF', 'Swps'), loc='upper left', title="$e="+str(NUM_EDITS)+"$", bbox_to_anchor=(1, 1.13))
else:
    legend = ax.legend( (rects1[0], rects2[0], rects3[0], rects4[0]), ('SHD', 'SeqAn', 'AF', 'Swps'), loc='upper left', title="$e="+str(NUM_EDITS) + "$", bbox_to_anchor=(1, 1.13))
plt.setp(legend.get_title(),fontsize='12')


#autolabel(rects1)
#autolabel(rects2)
# autolabel(rects4)

"""
output the pgf and pdflatex documents
"""
savefig(OUTPUT_FILE)

print "Done with " + OUTPUT_FILE + ".{pdf,pgf}"
