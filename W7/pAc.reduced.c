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
        char args[100];
        sprintf(args, "%d,%d %d,%d %d %s %d", p2c[0], p2c[1], c2p[0], c2p[1], BSIZE, buf, i);

        char *argv[] = {"./child", args, NULL};
        execv("./child", argv);
        perror("execv");
        exit(EXIT_FAILURE);
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
        read(c2p[0], buf, BSIZE);
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