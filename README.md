# MPI Stencil with Cartesian Topology

**Course:** Multicore Architecture and Programming  
**Problem ID:** 26  
**Team:** Abhijith, Basava, Darshan

## 1. Project Overview

This project implements a double-precision, two-dimensional five-point Jacobi stencil using C11 and MPI.

It compares two domain decomposition techniques: 1D horizontal-strip decomposition and 2D Cartesian decomposition. The project studies halo communication, communication-computation overlap, residual-checking overhead, strong scaling, and weak scaling.

## 2. Features and MPI Techniques

| Feature | Implementation / MPI Function |
|---|---|
| 2D Jacobi stencil | Five-point stencil |
| 1D decomposition | Horizontal-strip partitioning |
| 2D decomposition | Cartesian domain partitioning |
| Process topology | `MPI_Dims_create`, `MPI_Cart_create` |
| Neighbour discovery | `MPI_Cart_shift` |
| Blocking communication | `MPI_Sendrecv` |
| Non-blocking communication | `MPI_Irecv`, `MPI_Isend`, `MPI_Waitall` |
| Column exchange | `MPI_Type_vector` |
| Residual calculation | `MPI_Allreduce` with `MPI_MAX` |
| Performance measurement | `MPI_Wtime` |
| Experiment output | CSV files |
| Dataset generation | Python script |

Additional features include fixed zero boundary conditions, double-precision values, two grid buffers, configurable residual-check intervals, automated tests, experiment scripts, and graph generation.

## 3. Mathematical Model

The project uses a five-point Jacobi stencil. Each non-boundary cell is updated using its four neighbouring cells:

\[
u_{i,j}^{new} =
\frac{
u_{i-1,j}^{old} +
u_{i+1,j}^{old} +
u_{i,j-1}^{old} +
u_{i,j+1}^{old}
}{4}
\]

The residual is defined as the maximum absolute difference between consecutive iterations:

\[
R = \max_{i,j}
|u_{i,j}^{new}-u_{i,j}^{old}|
\]

Physical boundary cells remain fixed at zero. Two arrays are used to store the previous and next iteration values.

## 4. Domain Decomposition

| 1D Strip Decomposition | 2D Cartesian Decomposition |
|---|---|
| Divides the global grid into horizontal strips | Divides the global grid into rectangular blocks |
| Each process owns contiguous rows | Each process owns a local rectangular sub-grid |
| Uses top and bottom halo rows | Uses north, south, east and west halo regions |
| Exchanges rows using `MPI_Sendrecv` | Uses Cartesian neighbours for halo exchange |
| Supports non-blocking communication | Uses derived datatypes for column exchange |

## 5. Halo Exchange and Communication Overlap

The project supports two communication modes.

**Blocking mode:**
1. Exchange halo values.
2. Complete communication.
3. Compute the local stencil cells.
4. Apply fixed boundary conditions.
5. Calculate the residual when required.

**Non-blocking overlap mode:**

1. Post receives using `MPI_Irecv`.
2. Post sends using `MPI_Isend`.
3. Compute interior cells that do not depend on incoming halo data.
4. Complete communication using `MPI_Waitall`.
5. Compute boundary cells that depend on received halo values.
6. Apply fixed boundary conditions.
7. Calculate the residual when required.

The `MPI_Type_vector` derived datatype is used to exchange non-contiguous east-west columns without manually packing column data.

## 6. Synthetic Dataset

The project uses deterministic synthetic temperature-distribution datasets containing two Gaussian hot spots. These are generated mathematically and do not represent real-world temperature measurements.

The temperature distribution is:

\[
T(x,y)=
100e^{-\frac{(x-0.30)^2+(y-0.35)^2}{0.012}}
+
65e^{-\frac{(x-0.72)^2+(y-0.68)^2}{0.020}}
\]

All four physical boundaries are fixed to zero. Values are expressed in arbitrary temperature units.

### Available Datasets

| Dataset | Grid Size | Purpose |
|---|---:|---|
| `synthetic_temperature_32x32.csv` | 32 × 32 | Testing and correctness checks |
| `synthetic_temperature_256x256.csv` | 256 × 256 | Strong-scaling and overlap experiments |
| `synthetic_temperature_512x512.csv` | 512 × 512 | Larger experiments and weak scaling |

### Generate a Custom Dataset

```bash
python3 dataset/generate_dataset.py \
    --nx 256 \
    --ny 256 \
    --output dataset/custom_256x256.csv
```

## 7. Performance Experiments

The project supports the following experiments:

| Experiment | Process Configurations | Purpose |
|---|---|---|
| 1D strong scaling | 1, 2, 4, 8 | Study strip decomposition performance |
| 2D strong scaling | 1, 4, 9, 16 | Study Cartesian decomposition performance |
| Communication overlap | Blocking and overlap | Compare execution time |
| Residual-check overhead | K = 1, 2, 5, 10, 20 | Study the cost of `MPI_Allreduce` |
| Weak scaling | Increasing process counts | Study performance with increasing workload |
| Surface-to-volume analysis | 1D and 2D | Compare communication requirements |

Performance measurements are stored as CSV files under `results/raw/`. Graphs are generated using the Python plotting script.

## 8. Project Structure

| Directory / File | Description |
|---|---|
| `src/` | MPI implementation source files |
| `include/` | Header files |
| `dataset/` | Synthetic datasets and generator |
| `tests/` | Smoke, correctness and overlap tests |
| `scripts/` | Automated experiment scripts |
| `analysis/` | Graph generation scripts |
| `results/raw/` | Experiment CSV files |
| `results/figures/` | Generated performance graphs |
| `report/` | Project report and documentation |
| `Makefile` | Build configuration |
| `PROJECT_AUDIT.md` | Implementation requirement review |
| `FINAL_VALIDATION.md` | Testing and validation status |

## 9. Software Requirements

- C11 compiler
- MPI implementation (Open MPI)
- Python 3
- Matplotlib
- Pandas
- Make

Install the required packages on Ubuntu:

```bash
sudo apt update

sudo apt install -y \
    build-essential \
    openmpi-bin \
    libopenmpi-dev \
    python3 \
    python3-matplotlib \
    python3-pandas
```

## 10. Build and Test

Compile the project:

```bash
make clean
make
```

Run the test scripts:

```bash
chmod +x tests/*.sh scripts/*.sh

./tests/smoke_test.sh
./tests/correctness_test.sh
./tests/overlap_test.sh
```

Run the experiments:

```bash
./scripts/run_all.sh
```

Generate the performance graphs:

```bash
python3 analysis/plot_results.py
```


