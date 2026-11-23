#ifndef COMMON_H
#define COMMON_H
#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define IDX(g,i,j) ((size_t)(i) * (g)->pitch + (j))
#endif
