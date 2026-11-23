#include "decomposition.h"

static void split_dimension(int global, int ranks, int coordinate,
                            int *local, int *offset) {
    int base = global / ranks;
    int remainder = global % ranks;
    *local = base + (coordinate < remainder);
    *offset = coordinate * base + (coordinate < remainder ? coordinate : remainder);
}

int decomp_2d_cart_init(Decomposition *d, const Options *o, MPI_Comm world) {
    int periods[2] = {0, 0};
    d->dims[0] = 0;
    d->dims[1] = 0;
    MPI_Dims_create(d->size, 2, d->dims);
    MPI_Cart_create(world, 2, d->dims, periods, 0, &d->comm);
    if (d->comm == MPI_COMM_NULL)
        return -1;
    MPI_Comm_rank(d->comm, &d->rank);
    MPI_Cart_coords(d->comm, d->rank, 2, d->coords);
    MPI_Cart_shift(d->comm, 0, 1, &d->north, &d->south);
    MPI_Cart_shift(d->comm, 1, 1, &d->west, &d->east);
    split_dimension(o->nx, d->dims[0], d->coords[0], &d->local_nx, &d->row0);
    split_dimension(o->ny, d->dims[1], d->coords[1], &d->local_ny, &d->col0);
    return d->local_nx > 0 && d->local_ny > 0 ? 0 : -1;
}
