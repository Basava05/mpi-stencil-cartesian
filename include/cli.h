#ifndef CLI_H
#define CLI_H
#include "common.h"
typedef enum { DECOMP_1D, DECOMP_2D } DecompKind;
typedef enum { MODE_BLOCKING, MODE_OVERLAP } CommMode;
typedef struct { DecompKind decomp; CommMode mode; int nx, ny, max_iters, check_every; double tol; bool fixed_iters, verify; const char *csv, *input, *final_grid; int repeat; } Options;
void options_defaults(Options *o); int parse_options(int argc, char **argv, Options *o, int rank); void print_usage(const char *prog);
#endif
