#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "csipc.h"

int main(int argc, char *argv[]) {

    int fd;
    segment_T *sMem;

    char errorMsgBuf[80];

    if(argc != 2) {
        sprintf(errorMsgBuf, "%s: ERROR", argv[0]);
        errno=EINVAL;
        perror(errorMsgBuf);
        exit(EXIT_FAILURE);
    }

    fd = shm_open(argv[1], O_RDWR, 0600);
    if (fd == -1) {
        sprintf(errorMsgBuf, "%s: ERROR: %s", argv[1], "shm_open()");
        perror(errorMsgBuf);
        exit(EXIT_FAILURE);
    }

    if( ftruncate(fd, sizeof(segment_T)) ) {
        sprintf(errorMsgBuf, "%s: ERROR: %s", argv[1], "ftruncate()");
        perror(errorMsgBuf);
        exit(EXIT_FAILURE);
    }

    sMem = mmap(NULL, sizeof(segment_T), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    for(;;) {
        write(1, C_MSG, sizeof(C_MSG));
        if( (sMem->msgLength = read(0, sMem->msg, MSG_LENGTH_MAX)) <= 0 ) {
            sMem->orator = 1;
            break;
        }
        sMem->orator = 1;
        while(sMem->orator != 2);
        write(1, sMem->msg, sMem->msgLength);
    }

    exit(0);
}
