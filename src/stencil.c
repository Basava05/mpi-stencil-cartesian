#include "stencil.h"

static double update_cell(Grid *g, int i, int j) {
    double value = 0.25 * (g->old[IDX(g, i - 1, j)] + g->old[IDX(g, i + 1, j)]
                         + g->old[IDX(g, i, j - 1)] + g->old[IDX(g, i, j + 1)]);
    g->next[IDX(g, i, j)] = value;
    return fabs(value - g->old[IDX(g, i, j)]);
}

double stencil_region(Grid *g, int first_i, int last_i, int first_j, int last_j) {
    double maximum = 0.0;
    for (int i = first_i; i <= last_i; ++i)
        for (int j = first_j; j <= last_j; ++j) {
            double difference = update_cell(g, i, j);
            if (difference > maximum) maximum = difference;
        }
    return maximum;
}

double stencil_edges(Grid *g) {
    double maximum = 0.0;
    for (int i = 1; i <= g->nx; ++i)
        for (int j = 1; j <= g->ny; ++j)
            if (i == 1 || i == g->nx || j == 1 || j == g->ny) {
                double difference = update_cell(g, i, j);
                if (difference > maximum) maximum = difference;
            }
    return maximum;
}
