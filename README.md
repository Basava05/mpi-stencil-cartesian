# MPI Stencil with Cartesian Topology

Team 26 implementation of a double-precision 2-D five-point Jacobi stencil. It provides horizontal-strip and 2-D Cartesian decompositions, `MPI_Sendrecv` blocking halos, non-blocking overlap, vector column datatypes, residual reduction, CSV metrics, experiment scripts, plots, and a report template.

Build in Ubuntu/WSL with `sudo apt install -y build-essential openmpi-bin libopenmpi-dev python3 python3-matplotlib python3-pandas`, then `make`. Example: `mpirun --oversubscribe -np 4 ./build/mpi_stencil --decomp 2d --mode overlap --nx 256 --ny 256 --input dataset/synthetic_temperature_256x256.csv --max-iters 500 --check-every 10 --fixed-iters --csv results/raw/test.csv`.

`dataset/` contains deterministic **SYNTHETIC** temperature matrices and a generator. The rank-zero reader validates the requested CSV, then distributes only each rank's owned strip/block; it does not replicate the global dataset on every rank. Regenerate sizes with `python3 dataset/generate_dataset.py --nx N --ny M --output dataset/name.csv`.

Run `tests/*.sh`, then `scripts/run_all.sh`, followed by `python3 analysis/plot_results.py` in a Unix/WSL MPI environment. See `PROJECT_AUDIT.md` and `FINAL_VALIDATION.md` for current evidence and exact validation status. Submit a GitHub link to the repository root (code) and to `dataset/` (dataset).
