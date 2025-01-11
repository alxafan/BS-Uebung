#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// using gen- and showMsg to communicate termination. Could also be done with a single mutex,
// but this would require locking and unlocking each cycle
static int min, max, genMsg, showMsg;
// Only using one mutex, because the printing process needs both, which leads to deadlocks when using one mutex per variable
pthread_mutex_t rw_mutex=PTHREAD_MUTEX_INITIALIZER;


static void *generateAndDetermine(void *arg) {
  while(genMsg == 0) {
    int x = rand();
    pthread_mutex_lock(&rw_mutex);
    min = (x < min) ? x : min;
    max = (x > max) ? x : max;
    pthread_mutex_unlock(&rw_mutex);
  }
}

static void *showActualResults(void *arg) {
  while(showMsg == 0) {
    sleep(1);
    pthread_mutex_lock(&rw_mutex);
    printf("[%d, %d], %d, %d\n", 0, RAND_MAX, min, max);
    pthread_mutex_unlock(&rw_mutex);
  }
}

int main() {
  const int MAX_SIZE = 1000;
  char input[MAX_SIZE];
  min = rand();
  max = min;

  pthread_t genThread;
  pthread_t showThread;
  if(pthread_create(&genThread, NULL, generateAndDetermine, NULL) != 0) {
    fprintf (stderr, "Konnte generate Thread nicht erzeugen\n");
    exit (EXIT_FAILURE);
  }
  if(pthread_create(&showThread, NULL, showActualResults, NULL) != 0) {
    fprintf (stderr, "Konnte show Thread nicht erzeugen\n");
    exit (EXIT_FAILURE);
  }

  // Waits until a line is submitted, which happens on pressing enter
  fgets(input, MAX_SIZE, stdin);
  genMsg = 1;
  showMsg = 1;

  pthread_join(genThread, NULL);
  pthread_join(showThread, NULL);

  return EXIT_SUCCESS;

}