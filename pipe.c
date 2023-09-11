#include "headers.h"
#include "tools.h"

int pipeCommand(char *cmd) {
    char *commands[MAX_COMMANDS]; // Array to store individual commands
    int num_commands = 0;

    // Tokenize the input command by pipe symbol '|'
    char *token = strtok(cmd, "|");
    while (token != NULL) {
        trim(token);
        commands[num_commands++] = token;
        token = strtok(NULL, "|");
    }

    if (num_commands < 2) {
        // No pipes found, return 0
        return 0;
    }

    // Create an array of pipes
    int pipes[num_commands - 1][2];

    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_commands; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            if (i > 0) {
                // Redirect input from the previous pipe
                dup2(pipes[i - 1][0], STDIN_FILENO);
                close(pipes[i - 1][0]);
                close(pipes[i - 1][1]);
            }
            if (i < num_commands - 1) {
                // Redirect output to the next pipe
                dup2(pipes[i][1], STDOUT_FILENO);
                close(pipes[i][0]);
                close(pipes[i][1]);
            }

            Command *cmd = parseCommand(commands[i], " \t");
            if (execvp(cmd->argv[0], cmd->argv) == -1) {
                fprintf(stderr, "ERROR: '%s' is not a valid command\n", cmd->argv[0]);
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            if (i > 0) {
                // Close the previous pipe's read and write ends
                close(pipes[i - 1][0]);
                close(pipes[i - 1][1]);
            }
            waitpid(pid, NULL, 0);
        }
    }

    // Close all remaining pipe ends in the parent process
    for (int i = 0; i < num_commands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    return 1;
}
