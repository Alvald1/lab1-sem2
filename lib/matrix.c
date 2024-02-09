#include <stdio.h>
#include <stdlib.h>

#include "code_status.h"
#include "get_number.h"
#include "matrix.h"

int
init_matrix(Matrix* matrix, unsigned int m) {
    Line* lines = NULL;
    lines = (Line*)malloc(m * sizeof(Line));
    if (lines == NULL) {
        return BAD_ALLOC;
    }
    for (size_t i = 0; i < m; ++i) {
        init_line(lines + i);
    }
    matrix->m = m;
    matrix->lines = lines;
    return OK;
}

int
get_matrix(Matrix* matrix) {
    unsigned int m = 0;
    printf("Введите количество строк: ");
    if (get_unsigned_int(&m) == EOF) {
        return EOF;
    }
    if (init_matrix(matrix, m) == BAD_ALLOC) {
        dealloc_matrix(matrix, m);
        return BAD_ALLOC;
    }
    for (size_t i = 0; i < m; ++i) {
        switch (get_line(matrix->lines + i)) {
            case EOF: dealloc_matrix(matrix, i); return EOF;
            case BAD_ALLOC: dealloc_matrix(matrix, i); return BAD_ALLOC;
        }
    }
    return OK;
}

void
dealloc_matrix(const Matrix* matrix, unsigned int m) {
    for (size_t i = 0; i < m; ++i) {
        dealloc_line(matrix->lines + i);
    }
    free(matrix->lines);
}

void
print_matrix(const Matrix* matrix) {
    for (size_t i = 0; i < matrix->m; ++i) {
        print_line(matrix->lines + i);
        printf("\n");
    }
}
