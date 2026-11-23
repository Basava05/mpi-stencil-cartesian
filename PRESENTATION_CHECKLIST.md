# Demonstration checklist — Team 26

## What is implemented

- Five-point double-precision Jacobi stencil with reproducible initialization and zero global boundaries.
- 1-D horizontal-strip decomposition with halo rows.
- 2-D Cartesian communicator built with `MPI_Dims_create`, `MPI_Cart_create`, and `MPI_Cart_shift`.
- Blocking halos with `MPI_Sendrecv`.
- Overlap mode: post nonblocking receives/sends, compute the halo-independent interior, wait, then compute edges.
- 2-D column halos using `MPI_Type_vector`, with datatype cleanup via `MPI_Type_free`.
- Maximum-update residual combined through `MPI_Allreduce` and configurable `--check-every`.
- Fixed-iteration runs, timing, CSV output, tests, experiment runners, plots, and report template.

## What must be demonstrated before submission

1. Install or open a terminal that provides `mpicc` and `mpirun` (WSL Ubuntu with Open MPI is recommended).
2. Build with `make` and run all three scripts in `tests/`.
3. Run `scripts/run_all.sh`; it creates real CSV data in `results/raw/`.
4. Run `python3 analysis/plot_results.py`; it creates the required PNG graphs.
5. Add observations based on those measured files to `report/report.md`.

## Honest project state

The repository contains no performance numbers or result figures until the above MPI runs occur. This is deliberate: performance data must come from the actual laptop/cluster, not be invented.
