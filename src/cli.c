#include "cli.h"

void options_defaults(Options *o) {
    *o = (Options){DECOMP_2D, MODE_BLOCKING, 256, 256, 500, 10, 1e-8, false, false, NULL, NULL, NULL, 1};
}

void print_usage(const char *program) {
    printf("Usage: %s --decomp 1d|2d --mode blocking|overlap --nx N --ny N "
           "[--max-iters N --check-every N --tol X --fixed-iters --verify --input CSV --csv PATH --final-grid CSV]\n", program);
}

int parse_options(int argc, char **argv, Options *o, int rank) {
    for (int i = 1; i < argc; ++i) {
        const char *argument = argv[i];
        if (!strcmp(argument, "--fixed-iters")) { o->fixed_iters = true; continue; }
        if (!strcmp(argument, "--verify")) { o->verify = true; continue; }
        if (!strcmp(argument, "--help")) { if (rank == 0) print_usage(argv[0]); return 0; }
        if (++i >= argc) { if (rank == 0) fprintf(stderr, "Missing value for %s\n", argument); return -1; }
        const char *value = argv[i];
        if (!strcmp(argument, "--decomp")) o->decomp = !strcmp(value, "1d") ? DECOMP_1D : !strcmp(value, "2d") ? DECOMP_2D : -1;
        else if (!strcmp(argument, "--mode")) o->mode = !strcmp(value, "blocking") ? MODE_BLOCKING : !strcmp(value, "overlap") ? MODE_OVERLAP : -1;
        else if (!strcmp(argument, "--nx")) o->nx = atoi(value);
        else if (!strcmp(argument, "--ny")) o->ny = atoi(value);
        else if (!strcmp(argument, "--max-iters")) o->max_iters = atoi(value);
        else if (!strcmp(argument, "--check-every")) o->check_every = atoi(value);
        else if (!strcmp(argument, "--tol")) o->tol = strtod(value, NULL);
        else if (!strcmp(argument, "--csv")) o->csv = value;
        else if (!strcmp(argument, "--input")) o->input = value;
        else if (!strcmp(argument, "--final-grid")) o->final_grid = value;
        else if (!strcmp(argument, "--repeat")) o->repeat = atoi(value);
        else { if (rank == 0) fprintf(stderr, "Unknown option %s\n", argument); return -1; }
        if (o->decomp != DECOMP_1D && o->decomp != DECOMP_2D) return -1;
        if (o->mode != MODE_BLOCKING && o->mode != MODE_OVERLAP) return -1;
    }
    return o->nx > 0 && o->ny > 0 && o->max_iters > 0 && o->check_every > 0 && o->repeat > 0 ? 1 : -1;
}
