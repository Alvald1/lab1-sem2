#include <getopt.h>
#include <stdio.h>
#include "lib/code_status.h"
#include "lib/matrix.h"
#include "lib/task.h"

int
main(int argc, char* argv[]) {
    Matrix matrix, result;
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
    switch (task(&matrix, &result)) {
        case EOF: dealloc_matrix(&matrix); return 0;
        case BAD_ALLOC:
            dealloc_matrix(&matrix);
            printf("\nBAD_ALLOC");
            return 0;
        case BAD_FILE:
            dealloc_matrix(&matrix);
            printf("\nBAD_FILE");
            return 0;
    }
    if (print_matrix(&result) == BAD_FILE) {
        printf("\nBAD_FILE");
        dealloc_matrix(&matrix);
        dealloc_matrix(&result);
        return 0;
    }
    dealloc_matrix(&result);
    dealloc_matrix(&matrix);
    return 0;
}
