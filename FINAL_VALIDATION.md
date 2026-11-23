# Final validation status

## Commands actually run

- Static repository inspection of every tracked source, script, test, and document.
- `python dataset/generate_dataset.py` for 32x32, 256x256, and 512x512 CSVs.

## Environment

- Host inspected: Windows workspace; `gcc` present.
- MPI compiler/runtime: **NOT AVAILABLE** (`mpicc` and `mpirun` absent).
- Intended runtime: Ubuntu/WSL Open MPI.
- Hardware disclosure for report: Intel Core i3-1115G4, 2 physical cores / 4 logical processors. Eight, nine, and sixteen ranks are oversubscribed.

## Results

| Item | Status |
|---|---|
| C source static review | PARTIAL — issues recorded in `PROJECT_AUDIT.md` |
| Compilation | NOT RUN — Open MPI unavailable |
| MPI tests | NOT RUN — Open MPI unavailable |
| Experiments / CSVs | NOT RUN — no fabricated results |
| Figures | NOT RUN — no measured input CSVs |
| Dataset generation | PASS — three deterministic CSV matrices generated |

## Next commands in Ubuntu/WSL

```bash
sudo apt update
sudo apt install -y build-essential openmpi-bin libopenmpi-dev python3 python3-matplotlib python3-pandas
cd /mnt/d/mpi-stencil-cartesian
make clean && make
chmod +x tests/*.sh scripts/*.sh
./tests/smoke_test.sh
./tests/correctness_test.sh
./tests/overlap_test.sh
./scripts/run_all.sh
python3 analysis/plot_results.py
```

Upload two links required by the lecturer: the GitHub repository root for code, and the `dataset/` directory link for the synthetic dataset.
