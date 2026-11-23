#ifndef METRICS_H
#define METRICS_H
typedef struct { int iterations; double residual, elapsed, reduction_time, checksum; } Metrics;
double surface_to_volume(int nx, int ny, int pr, int pc);
#endif
