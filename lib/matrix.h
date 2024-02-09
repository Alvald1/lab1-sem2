#ifndef __MATRIX__
#define __MATRIX__

#include <stdio.h>
#include <stdlib.h>

#include "code_status.h"
#include "get_number.h"
#include "line.h"

typedef struct _matrix {
    unsigned int m;
    Line* lines;
} Matrix;

//return BAD_ALLOC or EOF or OK
int get_matrix(Matrix* matrix);
void dealloc_matrix(Matrix* matrix, unsigned int m);
void print_matrix(const Matrix* matrix);

#endif
