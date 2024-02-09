#include <stdio.h>

#include "lib/get_number.h"
#include "lib/struct.h"

int
main() {
    unsigned int m = 0;
    void* tmp = NULL;
    Matrix matrix;
    printf("Введите количество строк: ");
    if (get_unsigned_int(&m) == EOF) {
        return 0;
    }
    init_matrix(&matrix, m);

    return 0;
}