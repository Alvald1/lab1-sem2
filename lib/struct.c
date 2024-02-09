#include "struct.h"

void
init_matrix(Matrix* matrix, unsigned int m) {
    matrix->m = m;
    void* tmp = NULL;
    Line* lines = NULL;
    tmp = (Line*)malloc(m * (sizeof(Line)));
    if (tmp == NULL) {
        return BAD_ALLOC;
    }
    lines = tmp;
    for (size_t i = 0; i < m; ++i) {
        init_line(lines + i);
    }
    matrix->lines = lines;
    return OK;
}

void
init_line(Line* line) {
    line->n = 0;
    line->arr = NULL;
}

void
get_line(Line* line) {
    unsigned int n = 0;
    void* tmp = NULL;
    int* arr = NULL;
    printf("Введите длину строки: ");
    if (get_unsigned_int(&n) == EOF) {
        return NULL;
    }
    tmp = (int*)malloc(n * (sizeof(int)));
    if (tmp == NULL) {
        return NULL;
    }
    arr = tmp;
    int number = 0;
    for (size_t i = 0; i < n; ++i) {
        printf("Введите число: ");
        if (get_signed_int(&number) == EOF) {
            free(arr);
            return NULL;
        }
        *(arr + i) = number;
    }
    line->n = n;
    line->arr = arr;
}