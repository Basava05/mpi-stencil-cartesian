#include "csv_output.h"

void csv_write(const char *path, const Options *options,
               const Decomposition *decomposition, const Metrics *metrics) {
    if (!path) return;
    FILE *file = fopen(path, "a+");
    if (!file) { perror(path); return; }
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0)
        fprintf(file, "processes,decomp,mode,nx,ny,pr,pc,iterations,check_every,residual,seconds,reduction_seconds,checksum,surface_to_volume\n");
    fprintf(file, "%d,%s,%s,%d,%d,%d,%d,%d,%d,%.17g,%.9f,%.9f,%.17g,%.17g\n",
            decomposition->size, options->decomp == DECOMP_1D ? "1d" : "2d",
            options->mode == MODE_BLOCKING ? "blocking" : "overlap", options->nx, options->ny,
            decomposition->dims[0], decomposition->dims[1], metrics->iterations, options->check_every,
            metrics->residual, metrics->elapsed, metrics->reduction_time, metrics->checksum,
            surface_to_volume(options->nx, options->ny, decomposition->dims[0], decomposition->dims[1]));
    fclose(file);
}
