#ifndef __MATRIX__
#define __MATRIX__

#include "line.h"

typedef struct _matrix {
    size_t cnt_lines;
    char* file_name;
    Line* lines;
} Matrix;

typedef int (*fptr_read_line)(Line* line, size_t* offset, FILE* file);

int read_matrix(Matrix* matrix, char flag);
int init_matrix(Matrix* matrix, char flag, FILE** file);
void dealloc_matrix(const Matrix* matrix);
int print_matrix(const Matrix* matrix);

#endif