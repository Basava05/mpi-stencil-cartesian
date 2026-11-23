#include "cli.h"
#include "grid.h"
#include "decomposition.h"
#include "halo_exchange.h"
#include "stencil.h"
#include "residual.h"
#include "csv_output.h"
#include "dataset.h"

static double enforce_boundaries_and_residual(Grid *g, const Decomposition *d,
                                              const Options *o) {
    double maximum = 0.0;
    for (int i = 1; i <= g->nx; ++i) {
        for (int j = 1; j <= g->ny; ++j) {
            int global_i = d->row0 + i - 1;
            int global_j = d->col0 + j - 1;
            if (global_i == 0 || global_i == o->nx - 1 ||
                global_j == 0 || global_j == o->ny - 1)
                g->next[IDX(g, i, j)] = 0.0;
            double difference = fabs(g->next[IDX(g, i, j)] - g->old[IDX(g, i, j)]);
            if (difference > maximum) maximum = difference;
        }
    }
    return maximum;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    Options options;
    options_defaults(&options);
    int parse_status = parse_options(argc, argv, &options, world_rank);
    if (parse_status <= 0) {
        if (parse_status < 0 && world_rank == 0) print_usage(argv[0]);
        MPI_Finalize();
        return parse_status < 0 ? 2 : 0;
    }

    Decomposition decomposition;
    if (decomp_init(&decomposition, &options, MPI_COMM_WORLD) != 0) {
        if (world_rank == 0) fprintf(stderr, "Grid is too small for this process layout.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    Grid grid;
    if (grid_init(&grid, decomposition.local_nx, decomposition.local_ny,
                  decomposition.row0, decomposition.col0, options.nx, options.ny) != 0)
        MPI_Abort(MPI_COMM_WORLD, 2);
    if (options.input && dataset_load_distributed(options.input, &grid, &decomposition,
                                                  options.decomp, options.nx, options.ny,
                                                  world_rank) != 0) {
        if (world_rank == 0) fprintf(stderr, "Invalid dataset: %s\n", options.input);
        MPI_Abort(MPI_COMM_WORLD, 3);
    }

    MPI_Barrier(decomposition.comm);
    double started = MPI_Wtime(), reduction_time = 0.0, residual = 0.0, local_residual = 0.0;
    int iteration;
    for (iteration = 1; iteration <= options.max_iters; ++iteration) {
        if (options.mode == MODE_BLOCKING) {
            halo_blocking(&grid, &decomposition, options.decomp);
            stencil_region(&grid, 1, grid.nx, 1, grid.ny);
        } else {
            MPI_Request requests[8];
            MPI_Datatype column = MPI_DATATYPE_NULL;
            int request_count = halo_start(&grid, &decomposition, options.decomp, requests, &column);
            stencil_region(&grid, 2, grid.nx - 1, 2, grid.ny - 1);
            halo_finish(request_count, requests, &column);
            stencil_edges(&grid);
        }
        local_residual = enforce_boundaries_and_residual(&grid, &decomposition, &options);
        if (iteration % options.check_every == 0) {
            residual = global_residual(local_residual, decomposition.comm, &reduction_time);
            if (!options.fixed_iters && residual < options.tol) {
                grid_swap(&grid);
                break;
            }
        }
        grid_swap(&grid);
    }
    if (iteration > options.max_iters) iteration = options.max_iters;
    if (iteration % options.check_every != 0)
        residual = global_residual(local_residual, decomposition.comm, &reduction_time);

    double local_checksum = 0.0, checksum = 0.0;
    for (int i = 1; i <= grid.nx; ++i)
        for (int j = 1; j <= grid.ny; ++j) local_checksum += grid.old[IDX(&grid, i, j)];
    MPI_Allreduce(&local_checksum, &checksum, 1, MPI_DOUBLE, MPI_SUM, decomposition.comm);

    Metrics metrics = {iteration, residual, MPI_Wtime() - started, reduction_time, checksum};
    if (decomposition.rank == 0) {
        printf("MPI stencil: processes=%d decomp=%s grid=%dx%d process-grid=%dx%d mode=%s iterations=%d residual=%.3e time=%.6fs checksum=%.17g\n",
               decomposition.size, options.decomp == DECOMP_1D ? "1d" : "2d", options.nx, options.ny,
               decomposition.dims[0], decomposition.dims[1], options.mode == MODE_BLOCKING ? "blocking" : "overlap",
               iteration, residual, metrics.elapsed, checksum);
        if (options.verify) printf("Verification checksum: %.17g\n", checksum);
        csv_write(options.csv, &options, &decomposition, &metrics);
    }
    grid_free(&grid);
    decomp_free(&decomposition);
    MPI_Finalize();
    return 0;
}
