#include "matrix.h"
#include <readline/readline.h>
#include <stdarg.h>
#include <stdlib.h>
#include "code_status.h"
#include "get_number.h"
#include "stdio.h"

int init_matrix(Matrix* matrix, char flag, FILE** file, ...);

char*
get_file_name(const Matrix* matrix) {
    return matrix->file_name;
}

size_t
get_cnt_lines(const Matrix* matrix) {
    return matrix->cnt_lines;
}

Line*
get_lines(const Matrix* matrix) {
    return matrix->lines;
}

void
set_file_name(Matrix* matrix, char* file_name) {
    matrix->file_name = file_name;
}

void
set_cnt_lines(Matrix* matrix, size_t cnt_lines) {
    matrix->cnt_lines = cnt_lines;
}

void
set_lines(Matrix* matrix, Line* lines) {
    matrix->lines = lines;
}

int
print_matrix(const Matrix* matrix) {
    FILE* file = fopen(get_file_name(matrix), "rb");
    size_t cnt_lines = get_cnt_lines(matrix);
    Line* lines = get_lines(matrix);
    if (file == NULL) {
        return BAD_FILE;
    }
    for (size_t i = 0; i < cnt_lines; ++i) {
        if (print_line(lines + i, file) == BAD_FILE) {
            fclose(file);
            return BAD_FILE;
        }
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
init_matrix_with_len(Matrix* matrix, char flag, FILE** file, size_t len) {
    return init_matrix(matrix, flag, file, len);
}

int
init_matrix_without_len(Matrix* matrix, char flag, FILE** file) {
    return init_matrix(matrix, flag, file);
}

int
init_matrix(Matrix* matrix, char flag, FILE** file, ...) {
    size_t cnt_lines = 0;
    *file = NULL;
    va_list args;
    char* file_name = readline("Название файла: ");
    if (file_name == NULL) {
        return EOF;
    }
    if (flag == FMODE_ON) {
        *file = fopen(file_name, "rb");
    } else if (flag == FMODE_OFF) {
        *file = fopen(file_name, "wb");
    } else if (flag == FMODE_RES) {
        *file = fopen(file_name, "wb");
    }
    if (*file == NULL) {
        free(file_name);
        return BAD_FILE;
    }
    if (flag == FMODE_ON) {
        if (fread(&cnt_lines, sizeof(size_t), 1, *file) < 1) {
            free(file_name);
            fclose(*file);
            return BAD_FILE;
        }
    } else {
        if (flag == FMODE_OFF) {
            printf("Количество строк: ");
            if (get_unsigned_int(&cnt_lines) == EOF) {
                free(file_name);
                fclose(*file);
                return EOF;
            }
        } else if (flag == FMODE_RES) {
            va_start(args, file);
            cnt_lines = va_arg(args, size_t);
            va_end(args);
        }
        if (fwrite(&cnt_lines, sizeof(size_t), 1, *file) < 1) {
            free(file_name);
            fclose(*file);
            return BAD_FILE;
        }
    }
    Line* lines = (Line*)malloc(cnt_lines * sizeof(Line));
    if (lines == NULL) {
        free(file_name);
        fclose(*file);
        return BAD_ALLOC;
    }
    set_cnt_lines(matrix, cnt_lines);
    set_file_name(matrix, file_name);
    set_lines(matrix, lines);
    return OK;
}

int
read_matrix(Matrix* matrix, char flag) {
    size_t offset = sizeof(size_t);
    size_t cnt_lines = 0;
    fptr_read_line fptr = NULL;
    FILE* file = NULL;
    Line* lines = NULL;
    switch (init_matrix_without_len(matrix, flag, &file)) {
        case BAD_ALLOC: return BAD_ALLOC;
        case BAD_FILE: return BAD_FILE;
        case EOF: return EOF;
    }
    if (flag == FMODE_OFF) {
        fptr = read_line;
    } else if (flag == FMODE_ON) {
        fptr = read_line_file;
    }
    lines = get_lines(matrix);
    cnt_lines = get_cnt_lines(matrix);
    for (size_t i = 0; i < cnt_lines; ++i) {
        switch ((*fptr)(lines + i, &offset, file)) {
            case EOF:
                dealloc_matrix(matrix);
                fclose(file);
                return EOF;
            case BAD_FILE:
                dealloc_matrix(matrix);
                fclose(file);
                return BAD_FILE;
        }
    }
    fclose(file);
    return OK;
}