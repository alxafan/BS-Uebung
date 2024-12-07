#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  int p2c[2];
  int c2p[2];
  const int BSIZE = 100;
  char buf[BSIZE];
  int i;
  ssize_t nbytes;

    if (pipe(p2c) == -1 ) {
      perror("p2c pipe");
      exit(EXIT_FAILURE);
    }
    if (pipe(c2p) == -1 ) {
      perror("c2p pipe");
      exit(EXIT_FAILURE);
    }

    switch (fork()) {
      case -1:
        perror("fork");
        exit(EXIT_FAILURE);

      // CHILD PROCESS
      case 0:
        // close unused ends
        close(p2c[1]);
        close(c2p[0]);

        childloop:
        nbytes = read(p2c[0], buf, BSIZE);
        printf("Child: %d\n", *(int *)buf);
        i = *(int *)buf;
        if (i < 0) {
          exit(EXIT_SUCCESS);
        }
        else {
          i = i * 2;
          write(c2p[1], &i, sizeof(int));
          goto childloop;
        }

      // PARENT PROCESS
      default:
        // close unused ends
        close(p2c[0]);
        close(c2p[1]);
        i = 0;

        parentloop:
        i = i + 3;
        i = i * i;
        write(p2c[1], &i, sizeof(int));
        nbytes = read(c2p[0], buf, BSIZE);
        printf("Parent: %d\n", *(int *)buf);
        i = *(int *)buf;
        if (i < 1000) {
          goto parentloop;
        }
        i = -1;
        write(p2c[1], &i, sizeof(int));
        if (wait(NULL) < 0) {
          perror("wait");
          exit(EXIT_FAILURE);
        }
        close(p2c[1]);
        close(c2p[0]);
        exit(EXIT_SUCCESS);
  }
}