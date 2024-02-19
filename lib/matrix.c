#include "matrix.h"
#include <readline/readline.h>
#include <stdlib.h>
#include "code_status.h"
#include "get_number.h"
#include "stdio.h"

int
print_matrix(const Matrix* matrix) {
    FILE* file = fopen(matrix->file_name, "rb");
    if (file == NULL) {
        return BAD_FILE;
    }
    for (size_t i = 0; i < matrix->cnt_lines; ++i) {
        if (print_line(matrix->lines + i, file) == BAD_FILE) {
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
init_matrix(Matrix* matrix, char flag, FILE** file) {
    size_t cnt_lines = 0;
    *file = NULL;
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
    } else if (flag == FMODE_OFF) {
        printf("Количество строк: ");
        if (get_unsigned_int(&cnt_lines) == EOF) {
            free(file_name);
            fclose(*file);
            return EOF;
        }
        if (fwrite(&cnt_lines, sizeof(size_t), 1, *file) < 1) {
            free(file_name);
            fclose(*file);
            return BAD_FILE;
        }
    } else if (flag == FMODE_RES) {
        matrix->file_name = file_name;
        return OK;
    }
    Line* lines = (Line*)malloc(cnt_lines * sizeof(Line));
    if (lines == NULL) {
        free(file_name);
        fclose(*file);
        return BAD_ALLOC;
    }
    matrix->lines = lines;
    matrix->cnt_lines = cnt_lines;
    matrix->file_name = file_name;
    return OK;
}

int
read_matrix(Matrix* matrix, char flag) {
    size_t offset = sizeof(size_t);
    fptr_read_line fptr = NULL;
    FILE* file = NULL;
    switch (init_matrix(matrix, flag, &file)) {
        case BAD_ALLOC: return BAD_ALLOC;
        case BAD_FILE: return BAD_FILE;
        case EOF: return EOF;
    }
    if (flag == FMODE_OFF) {
        fptr = read_line;
    } else if (flag == FMODE_ON) {
        fptr = read_line_file;
    }
    for (size_t i = 0; i < matrix->cnt_lines; ++i) {
        switch ((*fptr)(matrix->lines + i, &offset, file)) {
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