#include "headers.h"
#include "tools.h"

void executeWithRedirection(char *command, char *inputFile, char *outputFile, int append) {
    // Create pipes for input and output redirection
    int inputPipe[2] = {-1, -1};
    int outputPipe[2] = {-1, -1};

    // Check if input file is specified
    if (inputFile != NULL) {
        int fd_in = open(inputFile, O_RDONLY);
        if (fd_in < 0) {
            perror("Error opening input file");
            return;
        }
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }

    // Check if output file is specified
    if (outputFile != NULL) {
        int flags = O_WRONLY | O_CREAT;
        if (append) {
            flags |= O_APPEND;
        } else {
            flags |= O_TRUNC;
        }
        int fd_out = open(outputFile, flags, 0644);
        if (fd_out < 0) {
            perror("Error opening output file");
            return;
        }
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }

    // Tokenize and execute the command
    char *parts[100];  // Adjust the size accordingly
    int numParts = 0;

    char *token = strtok(command, " ");
    while (token != NULL) {
        parts[numParts++] = token;
        token = strtok(NULL, " ");
    }

    // Ensure the last element is NULL for execvp
    parts[numParts] = NULL;

    // Execute the command
    execvp(parts[0], parts);
    // If execvp fails, perror will be called, and you can handle the error accordingly.
    perror("Error executing command");
    exit(EXIT_FAILURE);
}

void ioRedirectionHelper(char *command) {
    // Check if there is a pipe
    if (strchr(command, '|') != NULL) {
        pipeCommand(command);
        return;
    }

    // Initialize variables for redirection
    char *inputFile = NULL;
    char *outputFile = NULL;
    int append = 0; // Flag for append redirection

    // Tokenize the command to identify input and output redirection
    char *token = strtok(command, " ");
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            // Input redirection
            token = strtok(NULL, " "); // Get the input file name
            inputFile = token;
        } else if (strcmp(token, ">") == 0) {
            // Output redirection
            token = strtok(NULL, " "); // Get the output file name
            outputFile = token;
        } else if (strcmp(token, ">>") == 0) {
            // Append redirection
            token = strtok(NULL, " "); // Get the output file name
            outputFile = token;
            append = 1; // Set the append flag
        }
        token = strtok(NULL, " "); // Move to the next token
    }

    // Tokenize the command again to separate the command itself
    char *commandToExecute = strtok(command, "<>"); // Split by <, >, or >>
    trim(commandToExecute);

    // Execute the command with redirection
    executeWithRedirection(commandToExecute, inputFile, outputFile, append);
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