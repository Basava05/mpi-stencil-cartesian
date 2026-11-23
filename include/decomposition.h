#ifndef DECOMPOSITION_H
#define DECOMPOSITION_H
#include "cli.h"
typedef struct { MPI_Comm comm; int rank, size, dims[2], coords[2]; int north, south, west, east; int local_nx, local_ny, row0, col0; } Decomposition;
int decomp_init(Decomposition *d, const Options *o, MPI_Comm world); void decomp_free(Decomposition *d);
int decomp_2d_cart_init(Decomposition *d, const Options *o, MPI_Comm world);
#endif
