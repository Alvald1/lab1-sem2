#ifndef __LINE_MY__
#define __LINE_MY__

typedef struct _line {
    unsigned int n;
    int* arr;
} Line;

void init_line(Line* line);
//return BAD_ALLOC or EOF or OK
int get_line(Line* line);
void dealloc_line(const Line* line);
void print_line(const Line* line);

#endif
