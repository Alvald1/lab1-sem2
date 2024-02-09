#include "lib/get_number.h"
#include "lib/struct.h"

int
main() {
    Matrix matrix;
    switch (get_matrix(&matrix)) {
        case BAD_ALLOC: printf("BAD_ALLOC\n"); return 0;
        case EOF: return 0;
    }
    print_matrix(&matrix);
    dealloc_matrix(&matrix, matrix.m);
    return 0;
}