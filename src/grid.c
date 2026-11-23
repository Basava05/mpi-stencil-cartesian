#include "grid.h"

int grid_init(Grid *g, int nx, int ny, int row0, int col0, int global_nx, int global_ny) {
    g->nx = nx;
    g->ny = ny;
    g->pitch = ny + 2; /* one halo column on each side */
    size_t elements = (size_t)(nx + 2) * (size_t)g->pitch;
    g->old = calloc(elements, sizeof(double));
    g->next = calloc(elements, sizeof(double));
    if (!g->old || !g->next) {
        grid_free(g);
        return -1;
    }
    for (int i = 1; i <= nx; ++i) {
        for (int j = 1; j <= ny; ++j) {
            int global_i = row0 + i - 1;
            int global_j = col0 + j - 1;
            g->old[IDX(g, i, j)] = sin(0.031 * (global_i + 1)) * sin(0.047 * (global_j + 1));
            if (global_i == 0 || global_i == global_nx - 1 ||
                global_j == 0 || global_j == global_ny - 1)
                g->old[IDX(g, i, j)] = 0.0;
        }
    }
    return 0;
}

void grid_free(Grid *g) {
    free(g->old);
    free(g->next);
    memset(g, 0, sizeof(*g));
}

void grid_swap(Grid *g) {
    double *temporary = g->old;
    g->old = g->next;
    g->next = temporary;
}
