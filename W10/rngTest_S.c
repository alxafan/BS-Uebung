#include <stdio.h>
#include <stdlib.h>

void generateAndShow(unsigned long N) {
  int min = rand();
  int max = min;
  for(int i = 0; i < N-2; i++) {
      int x = rand();
      max = (x > max) ? x : max;
      min = (x < min) ? x : min;
    }
  // If N is 0 the number should be displayed immediately as per a):
  // "Vorgenannte Ausgabe soll erstmals nach N generierten Pseudozufalls zahlen und danach nach jeder weiteren erfolgen!"
  if (N == 0) printf("[%d, %d], %d, %d\n", 0, RAND_MAX, min, max); 
  while(1) {
    int x = rand();
    min = (x < min) ? x : min;
    max = (x > max) ? x : max;
    printf("[%d, %d], %d, %d\n", 0, RAND_MAX, min, max);
  }
}

int main() {
  generateAndShow(9999999999);
}