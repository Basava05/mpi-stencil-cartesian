#!/usr/bin/env bash
set -euo pipefail
a=$(mpirun --oversubscribe -np 4 ./build/mpi_stencil --decomp 2d --mode blocking --nx 32 --ny 32 --input dataset/synthetic_temperature_32x32.csv --max-iters 20 --fixed-iters --verify | awk '/checksum/{print $NF}')
b=$(mpirun --oversubscribe -np 4 ./build/mpi_stencil --decomp 2d --mode overlap --nx 32 --ny 32 --input dataset/synthetic_temperature_32x32.csv --max-iters 20 --fixed-iters --verify | awk '/checksum/{print $NF}')
python3 -c "import sys; assert abs(float('$a')-float('$b')) < 1e-10"
