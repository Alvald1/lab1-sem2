#ifndef __STRUCT__
#define __STRUCT__

#include <stdio.h>
#include <stdlib.h>

#include "get_number.h"

enum { OK, BAD_ALLOC };

typedef struct _matrix {
    unsigned int m;
    Line** lines;
} Matrix;

typedef struct _line {
    unsigned int n;
    int* arr;
} Line;

void init_matrix(Matrix* matrix, unsigned int m);
void init_line(Line* line);
void get_line(Line* line);

#endif