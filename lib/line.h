#ifndef __LINE_LIB__
#define __LINE_LIB__

#include <stdio.h>

typedef struct _line {
    size_t cnt_numbers;
    size_t offset;
} Line;

int read_line(Line* line, size_t* offset, FILE* file);
int print_line(const Line* line, FILE* file);
int read_line_file(Line* line, size_t* offset, FILE* file);

#endif