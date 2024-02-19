#include "task.h"
#include <stdlib.h>
#include "code_status.h"

size_t
find_max_ind(Line* line, FILE* file) {
    size_t max_ind = 0;
    int number = 0;
    int number_max = 0;
    fseek(file, line->offset, SEEK_SET);
    for (size_t i = 0; i < line->cnt_numbers; ++i) {
        if (fread(&number, sizeof(int), 1, file) < 1) {
            return BAD_FILE;
        }
        if (number >= number_max) {
            number_max = number;
            max_ind = i;
        }
    }
    return max_ind;
}

int
memcopy(FILE* file_dest, FILE* file_src, size_t offset_dest, size_t offset_src, size_t n) {
    fseek(file_dest, offset_dest, SEEK_SET);
    fseek(file_src, offset_src, SEEK_SET);
    int tmp = 0;
    for (size_t i = 0; i < n; ++i) {
        if (fread(&tmp, sizeof(int), 1, file_src) < 1) {
            return BAD_FILE;
        }
        if (fwrite(&tmp, sizeof(int), 1, file_dest) < 1) {
            return BAD_FILE;
        }
    }
    return OK;
}

int
task(Matrix* matrix, Matrix* result) {
    FILE *file_dest = NULL, *file_src = NULL;
    size_t offset = sizeof(size_t), max_ind = 0;
    switch (init_matrix(result, FMODE_RES, &file_dest)) {
        case BAD_FILE: dealloc_matrix(matrix); return BAD_FILE;
        case EOF: dealloc_matrix(matrix); return EOF;
    }
    result->cnt_lines = matrix->cnt_lines;
    Line* lines = (Line*)malloc(result->cnt_lines * sizeof(Line));
    if (lines == NULL) {
        dealloc_matrix(matrix);
        free(result->file_name);
        return BAD_ALLOC;
    }
    result->lines = lines;
    if (fwrite(&(result->cnt_lines), sizeof(size_t), 1, file_dest) < 1) {
        dealloc_matrix(matrix);
        dealloc_matrix(result);
        fclose(file_dest);
        return BAD_FILE;
    }
    file_src = fopen(matrix->file_name, "rb");
    if (file_src == NULL) {
        dealloc_matrix(matrix);
        dealloc_matrix(result);
        fclose(file_dest);
        return BAD_FILE;
    }
    for (size_t i = 0; i < result->cnt_lines; ++i) {
        max_ind = 1 + find_max_ind(matrix->lines + i, file_src);
        offset += sizeof(size_t);
        if (fwrite(&max_ind, sizeof(size_t), 1, file_dest) < 1) {
            dealloc_matrix(matrix);
            dealloc_matrix(result);
            fclose(file_dest);
            fclose(file_src);
            return BAD_FILE;
        }
        (result->lines + i)->offset = offset;
        (result->lines + i)->cnt_numbers = max_ind;
        if (memcopy(file_dest, file_src, offset, (matrix->lines + i)->offset, max_ind) == BAD_FILE) {
            dealloc_matrix(matrix);
            dealloc_matrix(result);
            fclose(file_dest);
            fclose(file_src);
            return BAD_FILE;
        }
        offset += max_ind * sizeof(int);
    }
    fclose(file_dest);
    fclose(file_src);
    return OK;
}