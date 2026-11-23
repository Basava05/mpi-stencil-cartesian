#!/usr/bin/env python3
"""Generate deterministic SYNTHETIC temperature fields for MPI stencil input."""
import argparse, csv, math
from pathlib import Path

def value(i, j, nx, ny):
    if i in (0, nx - 1) or j in (0, ny - 1): return 0.0
    x, y = j / (ny - 1), i / (nx - 1)
    return 100.0 * math.exp(-((x-.30)**2 + (y-.35)**2)/.012) + 65.0 * math.exp(-((x-.72)**2 + (y-.68)**2)/.020)

def write(nx, ny, output):
    if nx < 3 or ny < 3: raise ValueError("dimensions must be at least 3")
    with Path(output).open("w", newline="") as f:
        w=csv.writer(f); w.writerows(([f"{value(i,j,nx,ny):.12g}" for j in range(ny)] for i in range(nx)))

if __name__ == "__main__":
    p=argparse.ArgumentParser(description="Generate SYNTHETIC two-Gaussian temperature CSV")
    p.add_argument("--nx", type=int, required=True); p.add_argument("--ny", type=int, required=True); p.add_argument("--output", required=True)
    a=p.parse_args(); write(a.nx, a.ny, a.output)
