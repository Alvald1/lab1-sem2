#include <stdlib.h>

#include "code_status.h"
#include "matrix.h"
#include "task.h"

int memcopy(Line* dest, const Line* src, int n);
size_t find_max_ind(const Line* arr);

int
task(const Matrix* matrix, Matrix* result) {
    void* tmp = NULL;
    size_t max_ind = 0;
    tmp = (Line*)malloc(matrix->m * sizeof(Line));
    if (tmp == NULL) {
        free(result);
        return BAD_ALLOC;
    }
    result->m = matrix->m;
    result->lines = tmp;
    for (size_t i = 0; i < matrix->m; ++i) {
        max_ind = find_max_ind((matrix->lines) + i);
        if (memcopy((result->lines) + i, (matrix->lines) + i, max_ind + 1) == BAD_ALLOC) {
            dealloc_matrix(result, i);
            free(result);
            return BAD_ALLOC;
        }
    }
    return OK;
}

int
memcopy(Line* dest, const Line* src, int n) {
    void* tmp = NULL;
    tmp = (int*)malloc(n * (sizeof(int)));
    if (tmp == NULL) {
        return BAD_ALLOC;
    }
    dest->arr = tmp;
    dest->n = n;
    int *dest_arr = dest->arr, *src_arr = src->arr;
    while (n--) {
        *dest_arr++ = *src_arr++;
    }
    return OK;
}

size_t
find_max_ind(const Line* line) {
    size_t max_ind = line->n > 0 ? 0 : -1;
    for (size_t i = 1; i < line->n; ++i) {
        if (*((line->arr) + i) >= *((line->arr) + max_ind)) {
            max_ind = i;
        }
    }
    return max_ind;
}
