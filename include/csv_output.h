#ifndef CSV_OUTPUT_H
#define CSV_OUTPUT_H
#include "cli.h"
#include "decomposition.h"
#include "metrics.h"
void csv_write(const char *path, const Options *o, const Decomposition *d, const Metrics *m);
#endif
