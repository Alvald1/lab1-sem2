#include "line.h"
#include "code_status.h"
#include "get_number.h"

void
set_offset(Line* line, size_t offset) {
    line->offset = offset;
}

void
set_cnt_numbers(Line* line, size_t cnt_lines) {
    line->cnt_numbers = cnt_lines;
}

size_t
get_offset(const Line* line) {
    return line->offset;
}

size_t
get_cnt_numbers(const Line* line) {
    return line->cnt_numbers;
}

int
read_line_file(Line* line, size_t* offset, FILE* file) {
    size_t cnt_numbers = 0;
    if (fread(&cnt_numbers, sizeof(size_t), 1, file) < 1) {
        return BAD_FILE;
    }
    *offset += sizeof(size_t);
    set_cnt_numbers(line, cnt_numbers);
    set_offset(line, *offset);
    *offset += cnt_numbers * sizeof(int);
    fseek(file, cnt_numbers * sizeof(int), SEEK_CUR);
    return OK;
}

int
print_line(const Line* line, FILE* file) {
    int number = 0;
    size_t cnt_numbers = get_cnt_numbers(line);
    fseek(file, get_offset(line), SEEK_SET);
    for (size_t i = 0; i < cnt_numbers; ++i) {
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
    *offset += sizeof(size_t);
    for (size_t i = 0; i < cnt_numbers; ++i) {
        printf("Введите число: ");
        if (get_signed_int(&number) == EOF) {
            return EOF;
        }
        if (fwrite(&number, sizeof(int), 1, file) < 1) {
            return BAD_FILE;
        }
    }
    set_cnt_numbers(line, cnt_numbers);
    set_offset(line, *offset);
    *offset += cnt_numbers * sizeof(int);
    return OK;
}
