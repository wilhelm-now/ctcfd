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
    def plot_2d(z, title, x=None, y=None, x_label=None, y_label=None, z_label=None):
        if x is None or y is None:
            X, Y = np.meshgrid(np.arange(f_data.shape[0]),
                               np.arange(f_data.shape[1]))
            x = X if x is None else x
            y = Y if y is None else y
            x_label = "x" if x_label is None else x_label
            y_label = "y" if y_label is None else y_label

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
    if len(selections) == 0:
        print("Nothing to plot.")
        return

    for selection in selections:
        f_key, axes_keys = selection
        f_data = np.array(js[f_key])
        axes_data = [np.array(js[k]) for k in axes_keys]

        for i, d in enumerate(axes_data):
            if np.shape(f_data) != np.shape(d):
                raise ValueError(
                    f"Shape mismatch between shape({f_key})={np.shape(f_data)} and shape({axes_keys[i]})={np.shape(d)}")

        z_shape = np.shape(f_data)
        if len(z_shape) == 1:
            if len(axes_keys) == 0:
                plt.plot(f_data, label=f_key)
            elif len(axes_keys) == 1:
                plt.plot(axes_data[0], f_data, label=f"{f_key}({axes_keys[0]})")
            else:
                raise ValueError(f"Too many axes {len(axes_keys)} for 1-d plot")
            plt.legend()
            plt.title(f_key)
        elif len(z_shape) == 2:
            if len(axes_keys) == 0:
                plot_2d(f_data, title=f_key, z_label=f_key)
            elif len(axes_keys) == 2:
                x_label, y_label = axes_keys
                x_data, y_data = axes_data
                plot_2d(f_data, f"{f_key}({x_label}, {y_label})",
                        x=x_data, y=y_data,
                        x_label=x_label, y_label=y_label, z_label=f_key)

        # hack, but allow data to be stored in grid layout with multiple values
        elif len(z_shape) == 3:
            for idx in range(z_shape[-1]):
                plot_2d(f_data[:, :, idx],
                        f"{f_key}[:, :, {idx}]({axes_data[0]}, {axes_data[1]})",
                        x=axes_data[0], y=axes_data[1],
                        x_label=axes_keys[0], y_label=axes_keys[1])
        else:
            raise ValueError("Unsupported shape of data")

    plt.show()


def get_source(filename=None):
    if filename is None:
        inform("Reading input from stdin")
        return "".join(sys.stdin)
    with open(filename, 'r') as fo:
        return fo.read()


def get_selections(args):
    def balance_expressions(args):
        """'Collect arguments from balanced parentheses.
        ['f(x,', 'y)', 'z'] -> ['f(x,y)', 'z']
        """
        def grade(arg):
            return arg.count("(") - arg.count(")")
        out = []
        score = 0
        temp = ""
        for arg in args:
            score += grade(arg)
            if score == 0:
                out.append(temp + arg)
                temp = ""
            else:
                temp += arg
        if score > 0:
            raise ValueError("Unbalanced parentheses in input!")
        return out

    print("newer: ", balance_expressions(args))

    selections = []
    for group in balance_expressions(args):
        splat = group.split("(", 1)
        param = splat[0]
        arguments = splat[1].rsplit(")", 1)[0].split(",") if len(splat) > 1 else []
        selections.append((param, arguments))
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
    parser.add_argument("expressions", nargs='*',
                        help="Enter expressions like u(a, b) "
                        "to plot a dependent on a and b where "
                        "'u', 'a' and 'b' are keys present in the json data.")

    args = parser.parse_args()
    if args.quiet:
        VERBOSE = False
    do_plot(get_source(args.source), get_selections(args.expressions))
