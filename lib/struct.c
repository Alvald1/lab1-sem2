#include "struct.h"

void
init_matrix(Matrix* matrix, int m) {
    matrix->m = m;
    void* tmp = NULL;
    Line* lines = NULL;
    tmp = (Line*)malloc(m * (sizeof(Line)));
    if (tmp == NULL) {
        return BAD_ALLOC;
    }
    lines = tmp;
    for (int i = 0; i < m; ++i) {
        (lines + i)->n = 0;
        (lines + i)->arr = NULL;
    }
    matrix->lines = lines;
    return OK;
}