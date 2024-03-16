import sys
import subprocess as sp
import time

if __name__ == "__main__":
    args = sys.argv[1:]
    t0 = time.perf_counter()
    sp.call(args)
    t1 = time.perf_counter()
    print(f"command took {t1 - t0} second(?)")
