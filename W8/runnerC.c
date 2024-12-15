#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX_SIZE 1000

typedef struct linkedList {
    pid_t pid;
    char command[MAX_SIZE];
    struct linkedList *next;
} linkedList_T;

linkedList_T *head = NULL;
linkedList_T *tail = NULL;

void procAdd(pid_t pid, char *command) {
    linkedList_T *new_node = malloc(sizeof(linkedList_T));
    if (new_node == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    new_node->pid = pid;
    strncpy(new_node->command, command, sizeof(new_node->command) - 1);
    new_node->command[sizeof(new_node->command) - 1] = '\0';

    if (head == NULL) {
        head = new_node;
        tail = new_node;
    } else {
        tail->next = new_node;
        tail = new_node;
    }
}

void procShow() {
    linkedList_T *current = head;

    if (current == NULL) {
        printf("No active processes.\n> ");
        return;
    }

    printf("\nActive Processes:\n");
    printf("PID\tCommand\n");
    printf("-----------------------------------\n");

    while (current != NULL) {
        printf("%d\t%s\n", current->pid, current->command);
        current = current->next;
    }

    printf("-----------------------------------\n> ");
}

void procDel(pid_t pid) {
    linkedList_T *current = head;
    linkedList_T *prev = NULL;

    // No active processes
    if (current == NULL) {
        printf("No active processes.\n");
        return;
    }

    // If process was found at the head of the list
    if (current->pid == pid) {
        head = current->next;
        if (head == NULL) {
            tail = NULL;
        }
        free(current);
        printf("Process with PID %d was deleted from list.\n", pid);
        return;
    }

    // If process was found in the middle or at the end of the list
    while (current != NULL && current->pid != pid) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Process with PID %d not found.\n", pid);
        return;
    }

    prev->next = current->next;
    if (current == tail) {
        tail = prev;
    }

    free(current);
    printf("Process with PID %d was deleted from list.\n", pid);
}

int main() {
    const char *path_env = getenv("PATH");
    char input[MAX_SIZE];

    
    printf("> ");
    while (fgets(input, MAX_SIZE, stdin)) {
        
        // Remove newline character
        char *new_line = strchr(input, '\n');
        if (new_line != NULL) {
            *new_line = '\0';
        }

        char original_input[MAX_SIZE];
        strncpy(original_input, input, MAX_SIZE);

        // Get the command
        char *command = strtok(input, " ");

        // Check if the command is empty
        if (!command) {
            printf("Please enter a command.(Enter \"exit\" to exit)\n> ");
            continue;
        }

        if (strcmp(command, "exit") == 0) {
            printf("Terminating all child processes...\n");
            linkedList_T *current = head;

            while (current != NULL) {
                pid_t pid = current->pid;
                printf("Sent termination signal to child process %d\n", pid);   
                kill(pid, SIGTERM);

                sleep(2);
                
                 // We assume "minimal T = 2s" is a typo and what is meant is "maximal"
                int status;
                pid_t result = waitpid(pid, &status, WNOHANG);
                if (result == 0) {
                    printf("Child process %d is still running, sending kill signal.\n", pid);
                    kill(pid, SIGKILL);
                    waitpid(pid, &status, 0); // Wait for the process to terminate
                } else if (result == -1) {
                    perror("waitpid");
                } else {
                    printf("Child process %d was terminated.\n", pid);
                }
                // In the case of "minimal" we would instead to something like this:
                
                // waitpid(child_pids[i], &status, 0);
                // if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                //     printf("Child \"%d\" has exited successfully.\n", child_pids[i]);
                // } else {
                //     fprintf(stderr, "Error: Child \"%d\" has exited unsuccessfully.\n", child_pids[i]);
                // }
                current = current->next;
                procDel(pid);
            }

            printf("All child processes were terminated.\n");
            exit(0);
        }

        if (strcmp(command, "ps") == 0) { 
            procShow();
            continue;
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
                    procAdd(pid, original_input);
            }
        }

        free(path);

        printf("> ");
    }

    printf("Program beendet.\n");
    return 0;
}
