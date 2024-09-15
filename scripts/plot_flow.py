import matplotlib.pyplot as plt
import numpy as np
import json
import sys
import argparse


def int_suffix(text):
    """Return integer suffix of text. HELLO123 -> 123."""
    suffix = ""
    for c in reversed(text):
        if c.isdigit() or c == '-':
            suffix = c + suffix
        else:
            break
    if len(suffix) == 0:
        return None
    return int(suffix)


def get_source(filename=None):
    # copy paste from plot_json is evil but it's a tiny script
    # copy pasting to source makes script copy-pasteable
    if filename is None:
        print("Reading input from stdin")
        return "".join(sys.stdin)
    with open(filename, 'r') as fo:
        return fo.read()


def plot_flow(text, fname=None, timestep=None):
    """Plot a flowfield at specified timestep/iteration. If no
    timestep is provided max present is used.

    A flowfield is given by keys pTIMESTEP for pressure and vTIMESTEP
for velocity.
    x and y assumed to be present.
    """
    data = json.loads(text)

    if timestep is None:
        steps = set(map(int_suffix, data.keys()))
        candidates = sorted(
            filter(lambda step: all(map(
                lambda k: k + str(step) in data.keys(), ["p", "v"])), steps))
        if len(candidates) == 0:
            raise ValueError("No valid flowfield to default select")
        print("No timestep selected, candidate time steps:", candidates)
        timestep = candidates[-1]
        print("Using max step: ", timestep)

    x, y = data["x"], data["y"]
    p = data["p" + str(timestep)]
    v = np.array(data["v" + str(timestep)])  # as array for slicing later

    plt.contourf(x, y, p, alpha=0.5)
    plt.colorbar(label="Pressure")
    plt.quiver(x, y, v[:, :, 0], v[:, :, 1])
    plt.xlabel("X")
    plt.ylabel("Y")
    if fname is not None:
        plt.title(fname)
    plt.show()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Script to plot 2d flow vectors with pressure contours")
    parser.add_argument("-t", "--timestep", type=int, default=None,
                        help="timestep to plot flow at, "
                        "if not provided maximum in input is used.")
    parser.add_argument("--source", type=str, default=None,
                        help="filename of data to read, "
                        "if not present read from stdin")

    args = parser.parse_args()
    plot_flow(get_source(args.source), args.source, args.timestep)
