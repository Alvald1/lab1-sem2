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

enum { FMODE_ON, FMODE_OFF };

int read_matrix(Matrix* matrix, char flag);
int read_line(Line* line, size_t* offset, FILE* file);
int init_matrix(Matrix* matrix, char flag, FILE** file);
void dealloc_matrix(const Matrix* matrix);
int print_matrix(const Matrix* matrix);
int print_line(const Line* line, FILE* file);
int read_line_file(Line* line, size_t* offset, FILE* file);

typedef int (*fptr_read_line)(Line* line, size_t* offset, FILE* file);

int
main(int argc, char* argv[]) {
    Matrix matrix;
    char flag = 0;
    if (getopt(argc, argv, "f") == 'f') {
        flag = FMODE_ON;
    } else {
        flag = FMODE_OFF;
    }
    switch (read_matrix(&matrix, flag)) {
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
print_line(const Line* line, FILE* file) {
    int number = 0;
    fseek(file, line->offset, SEEK_SET);
    for (size_t i = 0; i < line->cnt_numbers; ++i) {
        if (fread(&number, sizeof(int), 1, file) < 1) {
            return BAD_FILE;
        }
        printf("%d ", number);
    }
    return OK;
}

int
print_matrix(const Matrix* matrix) {
    FILE* file = fopen(matrix->file_name, "rb");
    fseek(file, 0L, SEEK_SET);
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
            return EOF;
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
    if (fwrite(&cnt_numbers, sizeof(size_t), 1, file) < 1) {
        return BAD_FILE;
    }
    *(offset) += sizeof(size_t);
    for (size_t i = 0; i < cnt_numbers; ++i) {
        printf("Введите число: ");
        if (get_signed_int(&number) == EOF) {
            return EOF;
        }
        if (fwrite(&number, sizeof(int), 1, file) < 1) {
            return BAD_FILE;
        }
    }
    line->cnt_numbers = cnt_numbers;
    line->offset = *offset;
    *offset += cnt_numbers * sizeof(int);
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