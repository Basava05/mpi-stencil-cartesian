#ifndef HALO_EXCHANGE_H
#define HALO_EXCHANGE_H
#include "grid.h"
#include "decomposition.h"
void halo_blocking(const Grid *g, const Decomposition *d, DecompKind kind); int halo_start(const Grid *g, const Decomposition *d, DecompKind kind, MPI_Request req[8], MPI_Datatype *column); void halo_finish(int nreq, MPI_Request req[8], MPI_Datatype *column);
#endif
