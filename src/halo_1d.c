#include "halo_exchange.h"

enum { TO_NORTH = 11, TO_SOUTH = 12, TO_WEST = 13, TO_EAST = 14 };

void halo_blocking(const Grid *g, const Decomposition *d, DecompKind kind) {
    MPI_Sendrecv(&g->old[IDX(g, 1, 1)], g->ny, MPI_DOUBLE, d->north, TO_NORTH,
                 &g->old[IDX(g, g->nx + 1, 1)], g->ny, MPI_DOUBLE, d->south, TO_NORTH,
                 d->comm, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&g->old[IDX(g, g->nx, 1)], g->ny, MPI_DOUBLE, d->south, TO_SOUTH,
                 &g->old[IDX(g, 0, 1)], g->ny, MPI_DOUBLE, d->north, TO_SOUTH,
                 d->comm, MPI_STATUS_IGNORE);
    if (kind != DECOMP_2D) return;

    MPI_Datatype column;
    MPI_Type_vector(g->nx, 1, g->pitch, MPI_DOUBLE, &column);
    MPI_Type_commit(&column);
    MPI_Sendrecv(&g->old[IDX(g, 1, 1)], 1, column, d->west, TO_WEST,
                 &g->old[IDX(g, 1, g->ny + 1)], 1, column, d->east, TO_WEST,
                 d->comm, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&g->old[IDX(g, 1, g->ny)], 1, column, d->east, TO_EAST,
                 &g->old[IDX(g, 1, 0)], 1, column, d->west, TO_EAST,
                 d->comm, MPI_STATUS_IGNORE);
    MPI_Type_free(&column);
}

int halo_start(const Grid *g, const Decomposition *d, DecompKind kind,
               MPI_Request requests[8], MPI_Datatype *column) {
    int count = 0;
    MPI_Irecv(&g->old[IDX(g, 0, 1)], g->ny, MPI_DOUBLE, d->north, TO_SOUTH,
              d->comm, &requests[count++]);
    MPI_Irecv(&g->old[IDX(g, g->nx + 1, 1)], g->ny, MPI_DOUBLE, d->south, TO_NORTH,
              d->comm, &requests[count++]);
    MPI_Isend(&g->old[IDX(g, 1, 1)], g->ny, MPI_DOUBLE, d->north, TO_NORTH,
              d->comm, &requests[count++]);
    MPI_Isend(&g->old[IDX(g, g->nx, 1)], g->ny, MPI_DOUBLE, d->south, TO_SOUTH,
              d->comm, &requests[count++]);
    if (kind != DECOMP_2D) return count;

    MPI_Type_vector(g->nx, 1, g->pitch, MPI_DOUBLE, column);
    MPI_Type_commit(column);
    MPI_Irecv(&g->old[IDX(g, 1, 0)], 1, *column, d->west, TO_EAST, d->comm, &requests[count++]);
    MPI_Irecv(&g->old[IDX(g, 1, g->ny + 1)], 1, *column, d->east, TO_WEST, d->comm, &requests[count++]);
    MPI_Isend(&g->old[IDX(g, 1, 1)], 1, *column, d->west, TO_WEST, d->comm, &requests[count++]);
    MPI_Isend(&g->old[IDX(g, 1, g->ny)], 1, *column, d->east, TO_EAST, d->comm, &requests[count++]);
    return count;
}

void halo_finish(int count, MPI_Request requests[8], MPI_Datatype *column) {
    MPI_Waitall(count, requests, MPI_STATUSES_IGNORE);
    if (*column != MPI_DATATYPE_NULL) {
        MPI_Type_free(column);
        *column = MPI_DATATYPE_NULL;
    }
}
