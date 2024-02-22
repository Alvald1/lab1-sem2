#ifndef __MATRIX__
#define __MATRIX__

#include "line.h"

typedef struct _matrix {
    size_t cnt_lines;
    char* file_name;
    Line* lines;
} Matrix;

typedef int (*fptr_read_line)(Line* line, size_t* offset, FILE* file);

char* get_file_name(const Matrix* matrix);
size_t get_cnt_lines(const Matrix* matrix);
Line* get_lines(const Matrix* matrix);

void set_file_name(Matrix* matrix, char* file_name);
void set_cnt_lines(Matrix* matrix, size_t cnt_lines);
void set_lines(Matrix* matrix, Line* lines);

int read_matrix(Matrix* matrix, char flag);
int init_matrix_with_len(Matrix* matrix, char flag, FILE** file, size_t len);
int init_matrix_without_len(Matrix* matrix, char flag, FILE** file);
void dealloc_matrix(const Matrix* matrix);
int print_matrix(const Matrix* matrix);

#endif