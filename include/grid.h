#ifndef GRID_H
#define GRID_H
#include "common.h"
typedef struct { int nx, ny, pitch; double *old, *next; } Grid;
int grid_init(Grid *g, int nx, int ny, int global_row0, int global_col0, int global_nx, int global_ny); void grid_free(Grid *g); void grid_swap(Grid *g);
#endif
