#!/usr/bin/env bash
set -euo pipefail; mkdir -p results/raw
for p in 1 2 4 8; do mpirun --oversubscribe -np "$p" ./build/mpi_stencil --decomp 1d --mode blocking --nx $((128*p)) --ny 128 --max-iters 500 --check-every 10 --fixed-iters --csv results/raw/weak_scaling.csv; done
for p in 1 4 9 16; do case "$p" in 1)n=128;;4)n=256;;9)n=384;;16)n=512;;esac; mpirun --oversubscribe -np "$p" ./build/mpi_stencil --decomp 2d --mode blocking --nx "$n" --ny "$n" --max-iters 500 --check-every 10 --fixed-iters --csv results/raw/weak_scaling.csv; done
