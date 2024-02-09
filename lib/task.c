#include <stdlib.h>

#include "code_status.h"
#include "matrix.h"
#include "task.h"

int memcopy(Line* dest, const Line* src, unsigned int n);
size_t find_max_ind(const Line* arr);

int
task(const Matrix* matrix, Matrix* result) {
    size_t max_ind = 0;
    if (init_matrix(result, matrix->m) == BAD_ALLOC) {
        return BAD_ALLOC;
    }
    for (size_t i = 0; i < matrix->m; ++i) {
        max_ind = find_max_ind(matrix->lines + i);
        if (memcopy(result->lines + i, matrix->lines + i, max_ind + 1) == BAD_ALLOC) {
            dealloc_matrix(result, i);
            return BAD_ALLOC;
        }
    }
    return OK;
}

int
memcopy(Line* dest, const Line* src, unsigned int n) {
    int* arr = (int*)malloc(n * sizeof(int));
    if (arr == NULL) {
        return BAD_ALLOC;
    }
    dest->n = n;
    dest->arr = arr;
    for (size_t i = 0; i < n; ++i) {
        *(dest->arr + i) = *(src->arr + i);
    }
    return OK;
}

size_t
find_max_ind(const Line* line) {
    size_t max_ind = line->n > 0 ? 0 : -1;
    for (size_t i = 1; i < line->n; ++i) {
        if (*(line->arr + i) >= *(line->arr + max_ind)) {
            max_ind = i;
        }
    }
    return max_ind;
}
