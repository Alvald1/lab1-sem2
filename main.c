#include <stdio.h>

#include "lib/struct.h"
#include "lib/task.h"

int
main() {
    Matrix matrix, *result = NULL;
    switch (get_matrix(&matrix)) {
        case BAD_ALLOC: printf("\nBAD_ALLOC"); return 0;
        case EOF: return 0;
    }
    printf("\nИсходная матрица\n");
    print_matrix(&matrix);
    printf("\nРезультат\n");
    if (task(&matrix, &result) == BAD_ALLOC) {
        dealloc_matrix(&matrix, matrix.m);
        printf("\nBAD_ALLOC");
        return 0;
    }
    print_matrix(result);
    dealloc_matrix(&matrix, matrix.m);
    dealloc_matrix(result, result->m);
    free(result);
    return 0;
}
