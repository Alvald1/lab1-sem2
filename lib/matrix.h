#ifndef __MATRIX__
#define __MATRIX__

#include "line.h"

typedef struct _matrix {
    unsigned int m;
    Line* lines;
} Matrix;

//return BAD_ALLOC or EOF or OK
int get_matrix(Matrix* matrix);
void dealloc_matrix(const Matrix* matrix, unsigned int m);
void print_matrix(const Matrix* matrix);

#endif
