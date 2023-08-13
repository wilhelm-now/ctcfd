"""Plot script for 2d data input by json."""
import matplotlib.pyplot as plt
import numpy as np
import json
import sys
import argparse


VERBOSE = True


def inform(*args, **kwargs):
    if VERBOSE:
        print(*args, **kwargs)


def do_plot(json_text, selections):
    def plot_impl(x, y, z, title, x_label=None, y_label=None, z_label=None):
        fig = plt.figure()
        ax = fig.add_subplot(projection="3d")
        ax.plot_surface(x, y, z, cmap="viridis")
        ax.set_title(title)
        if x_label is not None:
            ax.set_xlabel(x_label)
        if y_label is not None:
            ax.set_ylabel(y_label)
        if z_label is not None:
            ax.set_zlabel(z_label)

    js = json.loads(json_text)
    inform("available keys: ", ", ".join(sorted(js.keys())))

    for selection in selections:

        f_key, x_key, y_key = selection
        x_data = np.array(js[x_key])
        y_data = np.array(js[y_key])
        z_data = np.array(js[f_key])

        z_shape = np.shape(z_data)
        # hack, but allow data to be stored in grid layout with multiple values
        if len(z_shape) == 3:
            for idx in range(z_shape[-1]):
                plot_impl(x_data, y_data, z_data[:, :, idx],
                          f"{f_key}[:, :, {idx}]({x_key}, {y_key})",
                          x_label=x_key, y_label=y_key)
        else:
            plot_impl(x_data, y_data, z_data,
                      f"{f_key}({x_key}, {y_key})",
                      x_label=x_key, y_label=y_key, z_label=f_key)

    plt.show()


def get_source(filename=None):
    if filename is None:
        inform("Reading input from stdin")
        return "".join(sys.stdin)
    with open(filename, 'r') as fo:
        return fo.read()


def get_selections(expressions_list):
    selections = []
    exp_text = "".join(expressions_list)
    for group in filter(len, exp_text.split(")")):
        dependent, args = group.split("(")
        selections.append([dependent] + list(
            map(lambda t: t.strip(), args.split(","))))
    return selections


def print_shapes(content: dict):
    for k, v in content.items():
        print(k, np.shape(v))
        print("\t", ", ".join(map(str, map(np.shape, v))))

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Script to plot 2d data in json format",
        epilog="For example \"python %s --source filename.txt u(x, y)\""
        % sys.argv[0].split('\\')[-1],
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument("--source", type=str, default=None,
                        help="filename of data to read, "
                        "if not present input is read from stdin.")
    parser.add_argument("-q", "--quiet", action="store_true",
                        help="Set this flag to disable informational prints")
    parser.add_argument("expressions", nargs='+',
                        help="Enter expressions like u(a, b) "
                        "to plot a dependent on a and b where "
                        "'u', 'a' and 'b' are keys present in the json data.")

    args = parser.parse_args()
    if args.quiet:
        VERBOSE = False
    do_plot(get_source(args.source), get_selections(args.expressions))
