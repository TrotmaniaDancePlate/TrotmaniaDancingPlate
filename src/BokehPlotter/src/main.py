import sys

from bokeh.colors import named as named_colors
from bokeh.palettes import Dark2_8
from bokeh.plotting import figure, output_file, show
from numpy import arange, loadtxt

from dataFunctions import data_functions

# Access parameters
if (len(sys.argv) > 1):
    fileName = sys.argv[1]
else:
    raise Exception("No filename supplied")

# colors to be used for plotting
colors = Dark2_8

def plot_function(function, graph, color):
    graph.line(x, function[1](data), line_width=2, legend_label=function[0], color=color)


def configure_graph():
    # create a new plot with a title and axis labels
    graph = figure(title="Dancing Plate Data",
                   x_axis_label='time',
                   y_axis_label='value',
                   x_range=(0, 1000),
                   plot_width=1000,
                   plot_height=800
                   )
    return graph


data = loadtxt(fileName)

# prepare x axis
x = arange(start=1, stop=len(data) + 1)

# output to static HTML file
output_file("plot.html")

graph = configure_graph()

for function, color in zip(data_functions(), colors):
    plot_function(function, graph, color)

graph.legend.click_policy = "hide"

# show the results
show(graph)
