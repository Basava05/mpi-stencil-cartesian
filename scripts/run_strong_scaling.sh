#!/usr/bin/env bash
set -euo pipefail
mkdir -p results/raw
for p in 1 2 4 8; do mpirun --oversubscribe -np "$p" ./build/mpi_stencil --decomp 1d --mode blocking --nx 256 --ny 256 --max-iters 500 --check-every 10 --fixed-iters --csv results/raw/strong_scaling_1d.csv; done
for p in 1 4 9 16; do mpirun --oversubscribe -np "$p" ./build/mpi_stencil --decomp 2d --mode blocking --nx 256 --ny 256 --max-iters 500 --check-every 10 --fixed-iters --csv results/raw/strong_scaling_2d.csv; done
