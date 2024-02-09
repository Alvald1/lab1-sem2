#include "line.h"

void
init_line(Line* line) {
    line->n = 0;
    line->arr = NULL;
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
dealloc_line(Line* line) {
    free(line->arr);
}

void
print_line(Line* line) {
    for (size_t i = 0; i < line->n; ++i) {
        printf("%d ", *((line->arr) + i));
    }
}
