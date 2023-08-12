"""Script to generate typelist with analytical solution for step4."""

try:
    import sympy
    from sympy.utilities.lambdify import lambdify
    HAS_SYMPY = True
except ImportError:
    HAS_SYMPY = False
    from math import exp, pi

import argparse

def make_func():
    """Return function for analytical solution at time t, location x and viscosity v."""
    if HAS_SYMPY:
        x, nu, t = sympy.symbols("x nu t")
        phi = (sympy.exp(-(x - 4 * t)**2 / (4 * nu * (t + 1))) +
               sympy.exp(-(x - 4 * t - 2 * sympy.pi)**2 / (4 * nu * (t + 1))))
        phiprime = phi.diff(x)
        
        u = -2 * nu * (phiprime/phi) + 4
        return lambdify((t, x, nu), u)
    else:
        def f(t, x, nu):
            # copied from print(u) when sympy was available
            return -2*nu*(-(-8*t + 2*x)*exp(-(-4*t + x)**2/(4*nu*(t + 1)))/(4*nu*(t + 1)) - (-8*t + 2*x - 4*pi)*exp(-(-4*t + x - 2*pi)**2/(4*nu*(t + 1)))/(4*nu*(t + 1)))/(exp(-(-4*t + x - 2*pi)**2/(4*nu*(t + 1))) + exp(-(-4*t + x)**2/(4*nu*(t + 1)))) + 4
        return f


def write_header(filename, nt, nx, lx, nu):
    """Write a header to filename with analytical solution at:
    time t
    dt time step size
    nt time step to save from
    nx points
    lx length of domain
    nu viscosity.
    """
    dx = lx/float(nx - 1)
    dt = dx*nu
    t = dt*nt
    analytical = make_func()
    xs = [i*dx for i in range(nx)]
    us = [analytical(t, x, nu) for x in xs]

    with open(filename, 'w') as fo:
        fo.write("""
#pragma once

#include <util/typelist.h>
#include <util/number.h>

typedef """)
        for u in us:
            fo.write(f"typelist<NUMBER_MAKE({u}), ")
        # close typelist
        fo.write("null_t")
        for _ in us:
            fo.write("> ")

        fo.write(f"analytical_t{nt};")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Script to make step4 analytical solution",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument("--NX", type=int, help="number of x gridpoints")
    parser.add_argument("--LX", type=float, default=2.0, help="length of x domain")
    parser.add_argument("--NU", type=float, help="viscosity")
    parser.add_argument("--NT",  type=int, default=0,
                        help="timestep to save values from")
    parser.add_argument("-o", "--filename", help="filename to save result in")

    args = parser.parse_args()

    write_header(args.filename, args.NT, args.NX, args.LX, args.NU)
