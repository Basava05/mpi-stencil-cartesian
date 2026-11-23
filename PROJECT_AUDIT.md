# Project audit — 2026-11-23

The repository was inspected before modification: all headers, C files, Makefile, tests, scripts, analysis program, documentation, and results directories were read. `results/raw` and `results/figures` contained only `.gitkeep`; therefore there are no real measurements or figures.

| Requirement | Evidence / relevant code | Status | Required correction or remaining evidence |
|---|---|---|---|
| A. 1D strips + `MPI_Sendrecv` | `decomp_1d.c`, `halo_1d.c:halo_blocking` | PARTIAL | Implementation exists; runtime deadlock/numerical test pending. |
| B. Cartesian topology | `decomp_2d_cart.c:decomp_2d_cart_init` | PARTIAL | `MPI_Dims_create`, `MPI_Cart_create`, `MPI_Cart_shift` used; runtime test pending. |
| C. Vector columns, no packing | `halo_1d.c` uses `MPI_Type_vector` | PARTIAL | Valid stride is `pitch`; communication needs MPI runtime test. |
| D. Genuine overlap | `halo_start`, `halo_finish`, `main.c` interior then edges | PARTIAL | Algorithm exists; needs small-block and numerical tests. |
| E. Residual/K/reduction cost | `residual.c`, `main.c` | PARTIAL | Final reduction is present; output schema needs richer timing fields and runtime validation. |
| F. required process counts | scripts | PARTIAL | Strong scripts specify counts; tests do not yet exercise all cases. |
| G. surface/volume graph | `analysis/plot_results.py` | NOT TESTED | Requires actual strong-scaling CSV. |
| H. overlap graph | `analysis/plot_results.py` | NOT TESTED | Requires repeated actual overlap CSV. |
| I. 1D vs 2D comparison | plot script + strong data | NOT TESTED | Requires 4-rank records. |
| J. weak-scaling graph | weak script + plot script | NOT TESTED | Must run sequentially with generated input sizes. |
| K. two-page report | `report/report.md` | PARTIAL | Method is present; measured results cannot be written until runs occur. |
| L. reproducible build | `Makefile`, `README.md` | PARTIAL | Correct Open MPI command path; no `mpicc` installed in this host. |

## Dataset audit

Added `dataset/` with deterministic numeric 32², 256², and 512² synthetic fields, generator, metadata, and documentation. `--input` was added. Input validation rejects missing/malformed/ragged/nonfinite data, incorrect matrix dimensions, and nonzero physical boundaries. Runtime validation is still NOT TESTED.

## Key limitations

This Windows workspace has no `mpicc` or `mpirun`; no compile, MPI test, graph, CSV measurement, or performance claim is marked PASS. The existing `--verify` is checksum output rather than the requested serial-reference verifier and remains a correctness gap to finish in an MPI environment.
