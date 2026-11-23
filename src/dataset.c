#include "dataset.h"

static int read_matrix(const char *path, double *matrix, int nx, int ny) {
    FILE *file = fopen(path, "r");
    if (!file) return -1;
    char line[65536];
    for (int i = 0; i < nx; ++i) {
        if (!fgets(line, sizeof(line), file)) { fclose(file); return -1; }
        char *cursor = line;
        for (int j = 0; j < ny; ++j) {
            char *end;
            double value = strtod(cursor, &end);
            if (end == cursor || !isfinite(value)) { fclose(file); return -1; }
            matrix[(size_t)i * ny + j] = value;
            cursor = end;
            if (j + 1 < ny) { if (*cursor != ',') { fclose(file); return -1; } ++cursor; }
        }
        while (*cursor == ' ' || *cursor == '\t' || *cursor == '\r' || *cursor == '\n') ++cursor;
        if (*cursor != '\0') { fclose(file); return -1; }
    }
    if (fgets(line, sizeof(line), file)) { fclose(file); return -1; }
    fclose(file);
    for (int i = 0; i < nx; ++i)
        for (int j = 0; j < ny; ++j)
            if ((i == 0 || j == 0 || i == nx - 1 || j == ny - 1) && matrix[(size_t)i * ny + j] != 0.0)
                return -1;
    return 0;
}

int dataset_load_distributed(const char *path, Grid *g, const Decomposition *d,
                             DecompKind kind, int nx, int ny, int world_rank) {
    int valid = 1;
    double *global = NULL;
    if (world_rank == 0) {
        global = malloc((size_t)nx * ny * sizeof(*global));
        valid = global && read_matrix(path, global, nx, ny) == 0;
    }
    MPI_Bcast(&valid, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (!valid) { free(global); return -1; }
    if (world_rank == 0) {
        for (int rank = 0; rank < d->size; ++rank) {
            int coords[2], rows, cols, row0, col0;
            if (kind == DECOMP_2D) MPI_Cart_coords(d->comm, rank, 2, coords);
            else { coords[0] = rank; coords[1] = 0; }
            int br = nx / d->dims[0], rr = nx % d->dims[0], bc = ny / d->dims[1], rc = ny % d->dims[1];
            rows = br + (coords[0] < rr); row0 = coords[0] * br + (coords[0] < rr ? coords[0] : rr);
            cols = bc + (coords[1] < rc); col0 = coords[1] * bc + (coords[1] < rc ? coords[1] : rc);
            if (rank == 0) for (int i = 0; i < rows; ++i) memcpy(&g->old[IDX(g, i + 1, 1)], &global[(size_t)(row0 + i) * ny + col0], (size_t)cols * sizeof(double));
            else for (int i = 0; i < rows; ++i) MPI_Send(&global[(size_t)(row0 + i) * ny + col0], cols, MPI_DOUBLE, rank, 71, MPI_COMM_WORLD);
        }
    } else for (int i = 0; i < g->nx; ++i) MPI_Recv(&g->old[IDX(g, i + 1, 1)], g->ny, MPI_DOUBLE, 0, 71, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    free(global);
    return 0;
}
