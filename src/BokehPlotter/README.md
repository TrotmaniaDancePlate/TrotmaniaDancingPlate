# Data Plotter

## Usage
The program uses pipenv for dependencies

Use `pipenv install` to install dependencies

Use `pipenv shell` to switch to the pipenv shell

Run program using `python3 src/main.py <file>` where file is your data you want to load from. The file has to have 1 value per row. Further details about the file format can be found here: [numpy.loadtxt](https://docs.scipy.org/doc/numpy/reference/generated/numpy.loadtxt.html)

The plot will open in your browser. If the browser does not open, open src/plot.html manually

## Adding a new data function
The plots generated are detemined by data function defined in the dataFunctions.py file. A data function will be given the array of data and must return an array of the same shape as the input array (a 1 dimensional array of numbers). Null values are allowed

To define a data function, add the function to dataFunctions.py and add a new line to the data_functions() file

`(<label>, <function_reference>)`

The label will be shown later in the plot. Currently a maximum of 8 data functions can be displayed at the same time, due to the used color palette. If you want to use more colors, chose a [color palette](https://docs.bokeh.org/en/latest/docs/reference/palettes.html) and assign it to the `colors` variable in the main.py