#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/code_status.h"
#include "lib/get_number.h"

typedef struct _line {
    size_t cnt_numbers;
    size_t offset;
} Line;

typedef struct _matrix {
    size_t cnt_lines;
    char* file_name;
    Line* lines;
} Matrix;

int read_matrix(Matrix* matrix);
int read_line(Line* line, size_t* offset, FILE* file);
int init_matrix(Matrix* matrix, size_t cnt_lines);
void dealloc_matrix(const Matrix* matrix);
int print_matrix(const Matrix* matrix);
void print_line(const Line* line, const FILE* file);

int
main() {
    Matrix matrix;
    switch (read_matrix(&matrix)) {
        case EOF: dealloc_matrix(&matrix); return 0;
        case BAD_ALLOC:
            dealloc_matrix(&matrix);
            printf("\nBAD_ALLOC");
            return 0;
    }
    dealloc_matrix(&matrix);

    return 0;
}

void
print_line(const Line* line, const FILE* file) {
    fseek(file, line->offset, SEEK_SET);
    for (size_t i = 0; i < line->cnt_numbers; ++i) {}
}

int
print_matrix(const Matrix* matrix) {
    FILE* file = fopen(matrix->file_name, "rb");
    if (file == NULL) {
        return BAD_FILE;
    }
    for (size_t i = 0; i < matrix->cnt_lines; ++i) {}
}

void
dealloc_matrix(const Matrix* matrix) {
    free(matrix->lines);
}

int
init_matrix(Matrix* matrix, size_t cnt_lines) {
    Line* lines = (Line*)malloc(cnt_lines * sizeof(Line));
    char* file_name = NULL;
    if (lines == NULL) {
        return BAD_ALLOC;
    }
    file_name = readline("Название файла: ");
    if (file_name == NULL) {
        dealloc_matrix(matrix);
        return EOF;
    }
    matrix->lines = lines;
    matrix->cnt_lines = cnt_lines;
    matrix->file_name = file_name;
    return OK;
}

int
read_line(Line* line, size_t* offset, FILE* file) {
    size_t cnt_numbers = 0;
    int number = 0;
    printf("Количество чисел: ");
    if (get_unsigned_int(&cnt_numbers) == EOF) {
        return EOF;
    }
    fwrite(&cnt_numbers, sizeof(int), 1, file);
    *(offset) += 1;
    for (size_t i = 0; i < cnt_numbers; ++i) {
        printf("Введите число: ");
        if (get_signed_int(&number) == EOF) {
            return EOF;
        }
        fwrite(&number, sizeof(int), 1, file);
    }
    line->cnt_numbers = cnt_numbers;
    line->offset = *offset;
    *offset += cnt_numbers;
    return OK;
}

int
read_matrix(Matrix* matrix) {
    size_t cnt_lines = 0, offset = 1;
    printf("Количество строк: ");
    if (get_unsigned_int(&cnt_lines) == EOF) {
        return EOF;
    }
    if (init_matrix(matrix, cnt_lines) == BAD_ALLOC) {
        return BAD_ALLOC;
    }
    FILE* file = fopen(matrix->file_name, "wb");
    fwrite(&cnt_lines, sizeof(int), 1, file);
    for (size_t i = 0; i < cnt_lines; ++i) {
        if (read_line(matrix->lines + i, &offset, file) == EOF) {
            dealloc_matrix(matrix);
            return EOF;
        }
    }
    fclose(file);
    return OK;
}