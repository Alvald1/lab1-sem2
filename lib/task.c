#include "task.h"
#include <stdlib.h>
#include "code_status.h"

size_t
find_max_ind(Line* line, FILE* file) {
    size_t max_ind = 0;
    size_t offset = get_offset(line);
    size_t cnt_numbers = get_cnt_numbers(line);
    int number = 0;
    int number_max = 0;
    fseek(file, offset, SEEK_SET);
    for (size_t i = 0; i < cnt_numbers; ++i) {
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
    size_t offset_dest = sizeof(size_t), max_ind = 0;
    size_t cnt_lines = get_cnt_lines(matrix);
    char* file_name = get_file_name(matrix);
    Line* lines_matrix = get_lines(matrix);
    Line* lines_result = NULL;
    switch (init_matrix_with_len(result, FMODE_RES, &file_dest, cnt_lines)) {
        case BAD_FILE: return BAD_FILE;
        case EOF: return EOF;
        case BAD_ALLOC: return BAD_ALLOC;
    }
    file_src = fopen(file_name, "rb");
    if (file_src == NULL) {
        dealloc_matrix(result);
        fclose(file_dest);
        return BAD_FILE;
    }
    lines_result = get_lines(result);
    for (size_t i = 0; i < cnt_lines; ++i) {
        max_ind = 1 + find_max_ind(lines_matrix + i, file_src);
        offset_dest += sizeof(size_t);
        if (fwrite(&max_ind, sizeof(size_t), 1, file_dest) < 1) {
            dealloc_matrix(result);
            fclose(file_dest);
            fclose(file_src);
            return BAD_FILE;
        }
        set_offset(lines_result + i, offset_dest);
        set_cnt_numbers(lines_result + i, max_ind);
        if (memcopy(file_dest, file_src, offset_dest, get_offset(lines_matrix + i), max_ind) == BAD_FILE) {
            dealloc_matrix(result);
            fclose(file_dest);
            fclose(file_src);
            return BAD_FILE;
        }
        offset_dest += max_ind * sizeof(int);
    }
    fclose(file_dest);
    fclose(file_src);
    return OK;
}