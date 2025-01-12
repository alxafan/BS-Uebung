#include <stdio.h>
#include <stdlib.h>

void generateAndShow(unsigned long N) {
    int min = rand();
    int max = min;

    if (N == 0) {
        printf("[%d, %d], %d, %d\n", 0, RAND_MAX, min, max);
    } else {
        for (int i = 0; i < N-1; i++) {
            int x = rand();
            max = (x > max) ? x : max;
            min = (x < min) ? x : min;
        }
    }

    // If N is 0 the number should be displayed immediately as per a):
    // "Vorgenannte Ausgabe soll erstmals nach N generierten Pseudozufalls zahlen und danach nach jeder weiteren erfolgen!"
    while(1) {
        int x = rand();
        min = (x < min) ? x : min;
        max = (x > max) ? x : max;
        printf("[%d, %d], %d, %d\n", 0, RAND_MAX, min, max);
    }
}

int main() {
    generateAndShow(1000);
    return 0;
}