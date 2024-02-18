#include <getopt.h>
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
void print_line(const Line* line, FILE* file);
int task(Matrix* matrix, Matrix* result);
int read_matrix_file(Matrix* matrix);
int read_line_file(Line* line, size_t* offset, FILE* file);

typedef int (*fptr_read)(Matrix* matrix);

int
main(int argc, char* argv[]) {
    Matrix matrix;
    fptr_read fptr = NULL;
    if (getopt(argc, argv, "f") == 'f') {
        fptr = read_matrix_file;
    } else {
        fptr = read_matrix;
    }
    switch ((*fptr)(&matrix)) {
        case EOF: return 0;
        case BAD_ALLOC: printf("\nBAD_ALLOC"); return 0;
        case BAD_FILE: printf("\nBAD_FILE"); return 0;
    }
    if (print_matrix(&matrix) == BAD_FILE) {
        printf("\nBAD_FILE");
        dealloc_matrix(&matrix);
        return 0;
    }
    dealloc_matrix(&matrix);
    return 0;
}

int
read_line_file(Line* line, size_t* offset, FILE* file) {
    if (fread(&(line->cnt_numbers), sizeof(size_t), 1, file) < 1) {
        return BAD_FILE;
    }
    *(offset) += sizeof(size_t);
    line->offset = *offset;
    *(offset) += (line->cnt_numbers) * sizeof(int);
    fseek(file, *offset, SEEK_SET);
    return OK;
}

int
read_matrix_file(Matrix* matrix) {
    FILE* file = NULL;
    size_t offset = 0;
    char* file_name = readline("Введите название файла: ");
    if (file_name == NULL) {
        return EOF;
    }
    file = fopen(file_name, "rb");
    if (file == NULL) {
        free(file_name);
        return BAD_FILE;
    }
    matrix->file_name = file_name;
    if (fread(&(matrix->cnt_lines), sizeof(size_t), 1, file) < 1) {
        free(file_name);
        fclose(file);
        return BAD_FILE;
    }
    offset += sizeof(size_t);
    matrix->lines = (Line*)malloc(matrix->cnt_lines * sizeof(Line));
    if (matrix->lines == NULL) {
        free(file_name);
        fclose(file);
        return BAD_ALLOC;
    }
    for (size_t i = 0; i < matrix->cnt_lines; ++i) {
        if (read_line_file(matrix->lines + i, &offset, file) == BAD_FILE) {
            dealloc_matrix(matrix);
            fclose(file);
            return BAD_FILE;
        }
    }
    return OK;
}

int
task(Matrix* matrix, Matrix* result) {
    init_matrix(result, matrix->cnt_lines);
    return OK;
}

void
print_line(const Line* line, FILE* file) {
    int number = 0;
    fseek(file, line->offset, SEEK_SET);
    for (size_t i = 0; i < line->cnt_numbers; ++i) {
        fread(&number, sizeof(int), 1, file);
        printf("%d ", number);
    }
}

int
print_matrix(const Matrix* matrix) {
    FILE* file = fopen(matrix->file_name, "rb");
    fseek(file, 0L, SEEK_SET);
    if (file == NULL) {
        return BAD_FILE;
    }
    for (size_t i = 0; i < matrix->cnt_lines; ++i) {
        print_line(matrix->lines + i, file);
        printf("\n");
    }
    fclose(file);
    return OK;
}

void
dealloc_matrix(const Matrix* matrix) {
    free(matrix->lines);
    free(matrix->file_name);
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
    fwrite(&cnt_numbers, sizeof(size_t), 1, file);
    *(offset) += sizeof(size_t);
    for (size_t i = 0; i < cnt_numbers; ++i) {
        printf("Введите число: ");
        if (get_signed_int(&number) == EOF) {
            return EOF;
        }
        fwrite(&number, sizeof(int), 1, file);
    }
    line->cnt_numbers = cnt_numbers;
    line->offset = *offset;
    *offset += cnt_numbers * sizeof(int);
    return OK;
}

int
read_matrix(Matrix* matrix) {
    size_t cnt_lines = 0, offset = 0;
    printf("Количество строк: ");
    if (get_unsigned_int(&cnt_lines) == EOF) {
        return EOF;
    }
    if (init_matrix(matrix, cnt_lines) == BAD_ALLOC) {
        return BAD_ALLOC;
    }
    FILE* file = fopen(matrix->file_name, "wb");
    fwrite(&cnt_lines, sizeof(size_t), 1, file);
    offset += sizeof(size_t);
    for (size_t i = 0; i < cnt_lines; ++i) {
        if (read_line(matrix->lines + i, &offset, file) == EOF) {
            dealloc_matrix(matrix);
            fclose(file);
            return EOF;
        }
    }
    fclose(file);
    return OK;
}