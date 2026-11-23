#include "decomposition.h"

static void split_dimension(int global, int ranks, int coordinate,
                            int *local, int *offset) {
    int base = global / ranks;
    int remainder = global % ranks;
    *local = base + (coordinate < remainder);
    *offset = coordinate * base + (coordinate < remainder ? coordinate : remainder);
}

int decomp_init(Decomposition *d, const Options *o, MPI_Comm world) {
    memset(d, 0, sizeof(*d));
    MPI_Comm_rank(world, &d->rank);
    MPI_Comm_size(world, &d->size);
    d->comm = world;
    d->north = d->south = d->west = d->east = MPI_PROC_NULL;

    if (o->decomp == DECOMP_2D)
        return decomp_2d_cart_init(d, o, world);

    d->dims[0] = d->size;
    d->dims[1] = 1;
    d->coords[0] = d->rank;
    split_dimension(o->nx, d->size, d->rank, &d->local_nx, &d->row0);
    d->local_ny = o->ny;
    d->col0 = 0;
    d->north = d->rank == 0 ? MPI_PROC_NULL : d->rank - 1;
    d->south = d->rank + 1 == d->size ? MPI_PROC_NULL : d->rank + 1;
    return d->local_nx > 0 ? 0 : -1;
}

void decomp_free(Decomposition *d) {
    if (d->comm != MPI_COMM_WORLD && d->comm != MPI_COMM_NULL)
        MPI_Comm_free(&d->comm);
}
