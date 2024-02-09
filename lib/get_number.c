#include "get_number.h"

#include <stdio.h>

int
get_number(const char* format, void* number) {
    int call_back = scanf(format, number);
    while (!call_back) {
        scanf("%*[^\n]");
        printf("Некорректный ввод\n");
        call_back = scanf(format, number);
    }
    return call_back;
}

int
get_signed_int(int* number) {
    return get_number("%d", number);
}

int
get_unsigned_int(int* number) {
    *number = 0;
    int call_back = get_number("%d", number);
    while (!(call_back) || *number < 0) {
        scanf("%*[^\n]");
        printf("Некорректный ввод\n");
        call_back = get_number("%d", number);
    }
    return call_back;
}
