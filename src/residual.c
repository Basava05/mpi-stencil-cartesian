#include "residual.h"

double global_residual(double local, MPI_Comm comm, double *seconds) {
    double started = MPI_Wtime();
    double global;
    MPI_Allreduce(&local, &global, 1, MPI_DOUBLE, MPI_MAX, comm);
    *seconds += MPI_Wtime() - started;
    return global;
}
