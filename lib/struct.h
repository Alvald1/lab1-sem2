#ifndef __STRUCT__
#define __STRUCT__

#include <stdio.h>
#include <stdlib.h>

enum { OK, BAD_ALLOC };

typedef struct _matrix {
    int m;
    Line** lines;
} Matrix;

typedef struct _line {
    int n;
    int* arr;
} Line;

void init_matrix(Matrix* matrix, int m);

#endif