import numpy


def data_functions():
    return [
        ("Value", apply_value_function),
        ("Gradient", apply_gradient_function),
        ("Delta", apply_delta_function)
    ]


def apply_value_function(data):
    return data


def apply_gradient_function(data):
    return numpy.gradient(data)


def apply_delta_function(data):
    data_delta = numpy.zeros_like(data)

    for idx, val in enumerate(data):
        if idx == 0:
            data_delta[idx] = 0
        else:
            data_delta[idx] = val - data[idx - 1]

    return data_delta
