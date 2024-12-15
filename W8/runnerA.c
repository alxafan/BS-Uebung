#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main() {
    const char *path_env = getenv("PATH");
    const int MAX_SIZE = 1000;
    char input[MAX_SIZE];

    int num_of_child_processes = 0;
    pid_t child_pids[MAX_SIZE];
    
    while (fgets(input, MAX_SIZE, stdin)) {
        
        // Remove newline character
        char *new_line = strchr(input, '\n');
        if (new_line != NULL) {
            *new_line = '\0';
        }

        char original_input[MAX_SIZE];
        strcpy(original_input, input);

        // Get the command
        char *command = strtok(input, " ");

        // Check if the command is empty
        if (!command) {
            printf("Please enter a command.(Enter \"exit\" to exit)\n> ");
            continue;
        }

        if (strcmp(command, "exit") == 0) {
            printf("Terminating all child processes...\n");

            for (int i = 0; i < num_of_child_processes; i++) {
                printf("Sent termination signal to child process %d\n", child_pids[i]);   
                kill(child_pids[i], SIGTERM);

                sleep(2);
                // We assume "minimal T = 2s" is a typo and what is meant is "maximal"
                int status;
                pid_t result = waitpid(child_pids[i], &status, WNOHANG);
                if (result == 0) {
                    printf("Child process %d is still running, sending kill signal.\n", child_pids[i]);
                    kill(child_pids[i], SIGKILL);
                    waitpid(child_pids[i], &status, 0); // Wait for the process to terminate
                } else if (result == -1) {
                    perror("waitpid");
                } else {
                    printf("Child process %d was terminated.\n", child_pids[i]);
                }
                // In the case of "minimal" we would instead to something like this:
                
                // waitpid(child_pids[i], &status, 0);
                // if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                //     printf("Child \"%d\" has exited successfully.\n", child_pids[i]);
                // } else {
                //     fprintf(stderr, "Error: Child \"%d\" has exited unsuccessfully.\n", child_pids[i]);
                // }
            }

            printf("All child processes were terminated.\n");
            return 0;
        }

        // Duplicate the PATH environment variable, because strtok modifies the string
        char *path = strdup(path_env);

        int command_found = 0;
        char *dir = strtok(path, ":");

        char command_path[MAX_SIZE];
        // Check each directory in the PATH
        while(dir != NULL) {
            snprintf(command_path, MAX_SIZE, "%s/%s", dir, command);

            // Command found
            if (access(command_path, 1) == 0) {
                command_found = 1;
                break;
            }

            // Command not found in this directory, try the next directory
            dir = strtok(NULL, ":");
        }

        // Command not found
        if (!command_found) {
            fprintf(stderr, "Error: Command \"%s\" wasn't found.\n", command);
        } else {
            printf("Command \"%s\" was found in: %s\n", command, dir);

            pid_t pid = fork();

            switch(pid) {
                case -1:
                    perror("fork");
                    exit(EXIT_FAILURE);
                // Child process
                case 0: {
                    int i = 0;
                    char *args[MAX_SIZE];

                    // Get the arguments
                    char *token = strtok(original_input, " ");
                    while (token != NULL) {
                        args[i++] = token;
                        token = strtok(NULL, " ");
                    }
                    args[i] = NULL;
                    
                    execv(command_path, args);
                    // Command could not be executed
                    perror("execv");
                    exit(1);
                }
                // Parent process
                default:
                    // Store the PIDs of the child processes
                    child_pids[num_of_child_processes++] = pid;
            }
        }
        
        free(path);
    }

    printf("Program beendet.\n");
    return 0;
}
