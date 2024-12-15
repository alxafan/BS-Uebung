#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_sigterm(int sig) {
    printf("Received SIGTERM, but ignoring it.\n");
}

int main() {
    // Set up the signal handler to ignore SIGTERM
    struct sigaction sa;
    sa.sa_handler = handle_sigterm;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGTERM, &sa, NULL);

    printf("Process %d is running and will ignore SIGTERM.\n", getpid());

    // Keep the process running indefinitely
    while (1) {
        sleep(1);
    }

    return 0;
}