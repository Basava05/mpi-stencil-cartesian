#include "metrics.h"

double surface_to_volume(int nx, int ny, int process_rows, int process_columns) {
    double one_way_halo_elements = 2.0 * ((process_rows - 1) * (double)ny +
                                          (process_columns - 1) * (double)nx);
    return one_way_halo_elements / ((double)nx * (double)ny);
}
