#include "headers.h"
#include "tools.h"

int ioRedirectionHelper(char *cmd) {
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
            perror("open");
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
            perror("open");
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
                perror("open");
                return 1;
            }
        }
    }

    // Tokenize the command by space to separate the command and its arguments
    char *token = strtok(cmd, " ");
    char *parts[100];  // Adjust the size accordingly
    int numParts = 0;

    while (token != NULL) {
        parts[numParts++] = token;
        token = strtok(NULL, " ");
    }

    // Ensure the last element is NULL for execvp
    parts[numParts] = NULL;

    // Redirect input if specified
    if (input_fd != STDIN_FILENO) {
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }

    // Redirect output if specified
    if (output_fd != STDOUT_FILENO) {
        if (append_flag) {
            dup2(output_fd, STDOUT_FILENO);
        } else {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }
    }

    // Execute the command
    execvp(parts[0], parts);
    // If execvp fails, perror will be called, and you can handle the error accordingly.
    perror("Error executing command");
    return 1;
}

void ioRedirection(char *cmd) {
//    fork and use ioRedirectionHelper in the child process
    pid_t pid = fork();
    if (pid == 0) {
        ioRedirectionHelper(cmd);
    } else if (pid > 0) {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("Error forking");
    }
}