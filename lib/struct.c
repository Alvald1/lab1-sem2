#include "struct.h"

void init_line(Line* line);
int get_line(Line* line);
void dealloc_line(Line* line);
void print_line(Line* line);
int init_matrix(Matrix* matrix, unsigned int m);

int
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
        switch (get_line((matrix->lines) + i)) {
            case EOF: dealloc_matrix(matrix, i); return EOF;
            case BAD_ALLOC: dealloc_matrix(matrix, i); return BAD_ALLOC;
        }
    }
    return OK;
}

int
get_line(Line* line) {
    unsigned int n = 0;
    void* tmp = NULL;
    int* arr = NULL;
    int number = 0;
    printf("Введите длину строки: ");
    if (get_unsigned_int(&n) == EOF) {
        return EOF;
    }
    tmp = (int*)malloc(n * (sizeof(int)));
    if (tmp == NULL) {
        return BAD_ALLOC;
    }
    arr = tmp;
    for (size_t i = 0; i < n; ++i) {
        printf("Введите число: ");
        if (get_signed_int(&number) == EOF) {
            free(arr);
            return EOF;
        }
        *(arr + i) = number;
    }
    line->n = n;
    line->arr = arr;
    return OK;
}

void
dealloc_matrix(Matrix* matrix, unsigned int m) {
    for (size_t i = 0; i < m; ++i) {
        dealloc_line(((matrix->lines) + i));
    }
    free(matrix->lines);
}

void
dealloc_line(Line* line) {
    free(line->arr);
}

void
print_matrix(Matrix* matrix) {
    for (size_t i = 0; i < matrix->m; ++i) {
        print_line((matrix->lines) + i);
        printf("\n");
    }
}

void
print_line(Line* line) {
    for (size_t i = 0; i < line->n; ++i) {
        printf("%d ", *((line->arr) + i));
    }
}