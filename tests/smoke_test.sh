#!/usr/bin/env bash
set -euo pipefail
mpirun --oversubscribe -np 1 ./build/mpi_stencil --decomp 1d --mode blocking --nx 32 --ny 32 --input dataset/synthetic_temperature_32x32.csv --max-iters 5 --fixed-iters --verify
