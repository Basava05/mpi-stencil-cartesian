#!/usr/bin/env bash
set -euo pipefail; mkdir -p results/raw
for d in 1d 2d; do for m in blocking overlap; do for p in 1 4; do mpirun --oversubscribe -np "$p" ./build/mpi_stencil --decomp "$d" --mode "$m" --nx 256 --ny 256 --max-iters 500 --check-every 10 --fixed-iters --csv results/raw/overlap.csv; done; done; done
