#include <stdio.h>

#include "lib/get_number.h"

int
main() {
    int m = 0;
    printf("Введите количество строк: ");
    if (get_number_int(&m) == EOF) {
        return 0;
    }

    return 0;
}