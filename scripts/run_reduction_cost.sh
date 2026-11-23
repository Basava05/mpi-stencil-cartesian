#!/usr/bin/env bash
set -euo pipefail; mkdir -p results/raw
for k in 1 2 5 10 20; do mpirun --oversubscribe -np 4 ./build/mpi_stencil --decomp 2d --mode blocking --nx 256 --ny 256 --max-iters 500 --check-every "$k" --fixed-iters --csv results/raw/reduction_cost.csv; done
