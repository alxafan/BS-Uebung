#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    const char *path_env = getenv("PATH");
    const int MAX_SIZE = 1000;
    char input[MAX_SIZE];
    
    printf("> ");
    while (fgets(input, MAX_SIZE, stdin)) {
        
        // Remove newline character
        char *new_line = strchr(input, '\n');
        if (new_line != NULL) {
            *new_line = '\0';
        }

        // Get the command
        char *command = strtok(input, " ");

        // Check if the command is empty
        if (!command) {
            printf("Bitte geben Sie einen Befehl ein.\n> ");
            continue;
        }

        // Duplicate the PATH environment variable, because strtok modifies the string
        char *path = strdup(path_env);

        int command_found = 0;
        char *dir = strtok(path, ":");

        // Check each directory in the PATH
        while(dir != NULL) {
            char command_path[MAX_SIZE];
            snprintf(command_path, MAX_SIZE, "%s/%s", dir, command);

            // Command found
            if (access(command_path, 1) == 0) {
                printf("Befehl \"%s\" gefunden in: %s\n", command, dir);
                command_found = 1;
                break;
            }

            // Command not found in this directory, try the next directory
            dir = strtok(NULL, ":");
        }

        // Command not found
        if (!command_found) {
            fprintf(stderr, "Fehler: Befehl \"%s\" nicht gefunden.\n", command);
        }

        free(path);

        printf("> ");
    }

    printf("Program beendet.\n");
    return 0;
}
