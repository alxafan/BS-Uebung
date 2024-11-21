#include <stdio.h>

int main() {
    int number;

    while (scanf("%d", &number) == 1) {
        printf("processNumber %d: %d\n", number, number*2);
    }

    return 0;
}

