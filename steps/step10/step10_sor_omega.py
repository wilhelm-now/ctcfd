from math import cos, pi, sqrt
import argparse


def write_omega_header(filename, paramname, nx, lx, ny, ly):
    """Plagiarize 
https://folk.ntnu.no/leifh/teaching/tkt4140/._main057.html
https://www.sciencedirect.com/science/article/pii/S0893965908001523#b2
page 153 https://books.google.no/books?hl=en&lr=&id=QpVpvE4gWZwC&oi=fnd&pg=PP1&ots=PhdPN6CkzT&sig=4V92euiUSSc-YEHcnqsXS01pxo8&redir_esc=y#v=onepage&q&f=false
page 540 https://books.google.no/books?hl=en&lr=&id=xi5omWiQ-3kC&oi=fnd&pg=PR5&ots=KmZP9u0qh8&sig=IqV8goWOAldJZnoBOeTXOIBEZqw&redir_esc=y#v=onepage&q&f=false
"""
    dx = lx / (nx - 1.0)
    dy = ly / (ny - 1.0)
    print(f"{nx=}, {lx=}, {dx=}")
    print(f"{ny=}, {ly=}, {dy=}")
    rho = (cos(pi/nx) + ((dx/dy)**2) * cos(pi/ny))/(1 + (dx/dy)**2)
    print(f"{rho=}")
    # rho = 0.5*(cos(pi/nx) + cos(pi/ny))
    # print(f"{rho=}")
    optimal_omega = 2/(1 + sqrt(1 - rho**2))
    print(f"{optimal_omega=}")

    with open(filename, 'w') as fo:
        fo.write("#pragma once\n")
        fo.write("// Optimal omega for successive over relaxation\n")
        fo.write(f"#define {paramname} ({optimal_omega})")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Script to make step9 sor header",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument("--NX", type=int, help="number of x gridpoints")
    parser.add_argument("--LX", type=float, default=2.0, help="length of x domain")
    parser.add_argument("--NY", type=int, help="number of y gridpoints")
    parser.add_argument("--LY", type=float, default=1.0, help="length of y domain")
    parser.add_argument("-n", "--paramname", help="parameter name to save variable in")
    parser.add_argument("-o", "--filename", help="filename to save result in")

    args = parser.parse_args()

    write_omega_header(args.filename, args.paramname, args.NX, args.LX, args.NY, args.LY)
