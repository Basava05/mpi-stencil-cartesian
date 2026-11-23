#ifndef DATASET_H
#define DATASET_H
#include "grid.h"
#include "decomposition.h"
int dataset_load_distributed(const char *path, Grid *grid, const Decomposition *decomp,
                             DecompKind kind, int global_nx, int global_ny, int world_rank);
#endif
