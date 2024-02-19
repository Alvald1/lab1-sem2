#include "line.h"
#include "code_status.h"
#include "get_number.h"

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
