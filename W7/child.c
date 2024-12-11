#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int p2c[2], c2p[2];
    int BSIZE;
    char buf[100];
    int i;

    sscanf(argv[1], "%d,%d %d,%d %d %s %d", &p2c[0], &p2c[1], &c2p[0], &c2p[1], &BSIZE, buf, &i);

  // close unused ends
  close(p2c[1]);
  close(c2p[0]);

  childloop:
    read(p2c[0], buf, BSIZE);
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