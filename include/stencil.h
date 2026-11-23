#ifndef STENCIL_H
#define STENCIL_H
#include "grid.h"
double stencil_region(Grid *g, int i0, int i1, int j0, int j1); double stencil_edges(Grid *g);
#endif
