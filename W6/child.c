#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 7) {
    fprintf(stderr, "Usage: %s p2c c2p BSIZE buf nbytes i\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  int p2c[2], c2p[2];
  sscanf(argv[1], "%d,%d", &p2c[0], &p2c[1]);
  sscanf(argv[2], "%d,%d", &c2p[0], &c2p[1]);
  const int BSIZE = atoi(argv[3]);
  char *buf = argv[4];
  ssize_t nbytes = atol(argv[5]);
  int i = atoi(argv[6]);

  // close unused ends
  close(p2c[1]);
  close(c2p[0]);

  childloop:
    nbytes = read(p2c[0], buf, BSIZE);
    printf("Child: %d\n", *(int *)buf);
    i = *(int *)buf;
    if (i < 0) {
      close(p2c[0]);
      close(c2p[1]);
      exit(EXIT_SUCCESS);
    }
    else {
      i = i * 2;
      write(c2p[1], &i, sizeof(int));
      goto childloop;
    }
}