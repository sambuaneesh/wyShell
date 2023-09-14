#include "headers.h"
#include "tools.h"

int pipeCommand(char *cmd) {
    char *commands[MAX_COMMANDS]; // Array to store individual commands
    int num_commands = 0;

//    return error if there is nothing to the left or to the right of a pipe
    if (cmd[0] == '|' || cmd[strlen(cmd) - 1] == '|') {
        printError("ERROR: Invalid use of pipe\n");
        return 1;
    }

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
            printError("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_commands; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            printError("fork");
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
                printError("ERROR: '%s' is not a valid command\n", cmd->argv[0]);
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


int ioPipe(char *cmd) {
    char *commands[MAX_COMMANDS]; // Array to store individual commands
    int num_commands = 0;
    int input_fd = STDIN_FILENO;  // Default input is stdin
    int output_fd = STDOUT_FILENO; // Default output is stdout
    int append_flag = 0;          // Flag for append mode

    // Check for input redirection
    char *input_file = strchr(cmd, '<');
    if (input_file != NULL) {
        *input_file = '\0'; // Split the command at the '<' character
        input_file = strtok(input_file + 1, " \t"); // Get the input file name
        input_fd = open(input_file, O_RDONLY);
        if (input_fd == -1) {
            printError("open");
            return 1;
        }
    }

    // Check for output redirection (including append mode)
    char *output_file = strstr(cmd, ">>");
    if (output_file != NULL) {
        *output_file = '\0'; // Split the command at ">>"
        output_file = strtok(output_file + 2, " \t"); // Get the output file name
        output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0666);
        if (output_fd == -1) {
            printError("open");
            return 1;
        }
        append_flag = 1; // Set the append flag
    } else {
        output_file = strchr(cmd, '>');
        if (output_file != NULL) {
            *output_file = '\0'; // Split the command at the '>' character
            output_file = strtok(output_file + 1, " \t"); // Get the output file name
            output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (output_fd == -1) {
                printError("open");
                return 1;
            }
        }
    }

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
            printError("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_commands; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            printError("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            if (i > 0) {
                // Redirect input from the previous pipe
                dup2(pipes[i - 1][0], STDIN_FILENO);
                close(pipes[i - 1][0]);
                close(pipes[i - 1][1]);
            } else {
                // Redirect input from the specified file (if any)
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            if (i < num_commands - 1) {
                // Redirect output to the next pipe
                dup2(pipes[i][1], STDOUT_FILENO);
                close(pipes[i][0]);
                close(pipes[i][1]);
            } else {
                // Redirect output to the specified file (if any)
                if (append_flag) {
                    dup2(output_fd, STDOUT_FILENO);
                } else {
                    dup2(output_fd, STDOUT_FILENO);
                    close(output_fd);
                }
            }

            Command *cmd = parseCommand(commands[i], " \t");
            if (execvp(cmd->argv[0], cmd->argv) == -1) {
                printError("ERROR: '%s' is not a valid command\n", cmd->argv[0]);
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

