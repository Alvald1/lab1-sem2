#ifndef __STRUCT__
#define __STRUCT__

#include <stdio.h>
#include <stdlib.h>

#include "get_number.h"

enum { OK, BAD_ALLOC };

typedef struct _line {
    unsigned int n;
    int* arr;
} Line;

typedef struct _matrix {
    unsigned int m;
    Line* lines;
} Matrix;

//return BAD_ALLOC or EOF or OK
int get_matrix(Matrix* matrix);

void dealloc_matrix(Matrix* matrix, unsigned int m);
void print_matrix(Matrix* matrix);

#endif