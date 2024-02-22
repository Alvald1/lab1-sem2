#ifndef __LINE_LIB__
#define __LINE_LIB__

#include <stdio.h>

typedef struct _line {
    size_t cnt_numbers;
    size_t offset;
} Line;

size_t get_cnt_numbers(const Line* line);
size_t get_offset(const Line* line);

void set_offset(Line* line, size_t offset);
void set_cnt_numbers(Line* line, size_t cnt_lines);

int read_line(Line* line, size_t* offset, FILE* file);
int print_line(const Line* line, FILE* file);
int read_line_file(Line* line, size_t* offset, FILE* file);

#endif