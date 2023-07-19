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
    js = json.loads(json_text)
    inform("available keys: ", ", ".join(sorted(js.keys())))

    for selection in selections:
        fig = plt.figure()
        ax = fig.add_subplot(projection="3d")
        f_key, x_key, y_key = selection
        ax.plot_surface(
            np.array(js[x_key]),
            np.array(js[y_key]),
            np.array(js[f_key]),
            cmap="viridis")
        ax.set_xlabel(x_key)
        ax.set_ylabel(y_key)
        ax.set_zlabel(f_key)
        ax.set_title(f"{f_key}({x_key}, {y_key})")

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
