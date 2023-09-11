#include "headers.h"
#include "tools.h"

void writeFile(char *command) {
    char *commandCopy = malloc(strlen(command) + 1);
    strcpy(commandCopy, command);

    char *token = strtok(commandCopy, ">");
    char *commandToExecute = malloc(strlen(token) + 1);
    strcpy(commandToExecute, token);

    token = strtok(NULL, ">");
    char *fileName = malloc(strlen(token) + 1);
    strcpy(fileName, token);

    trim(commandToExecute);
    trim(fileName);

    int fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error opening file");
        return;
    }

    int stdoutCopy = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    close(fd);

    executeCommand(commandToExecute, 0);

    dup2(stdoutCopy, STDOUT_FILENO);
    close(stdoutCopy);

    free(commandCopy);
    free(commandToExecute);
    free(fileName);
}

void readFile(char *command) {
    char *commandCopy = malloc(strlen(command) + 1);
    strcpy(commandCopy, command);

    char *token = strtok(commandCopy, "<");
    char *commandToExecute = malloc(strlen(token) + 1);
    strcpy(commandToExecute, token);

    token = strtok(NULL, "<");
    char *fileName = malloc(strlen(token) + 1);
    strcpy(fileName, token);

    trim(commandToExecute);
    trim(fileName);

    int fd = open(fileName, O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        return;
    }

    int stdinCopy = dup(STDIN_FILENO);
    dup2(fd, STDIN_FILENO);
    close(fd);

    executeCommand(commandToExecute, 0);

    dup2(stdinCopy, STDIN_FILENO);
    close(stdinCopy);

    free(commandCopy);
    free(commandToExecute);
    free(fileName);
}

void ioRedirection(char *command) {
//    check if there is a pipe
    if (strchr(command, '|') != NULL) {
        pipeCommand(command);
        return;
    }

    if (strchr(command, '<') != NULL) {
        readFile(command);
        return;
    }

//    if there is a >>, append to file
    if (strstr(command, ">>") != NULL) {
        char *commandCopy = malloc(strlen(command) + 1);
        strcpy(commandCopy, command);

        char *token = strtok(commandCopy, ">>");
        char *commandToExecute = malloc(strlen(token) + 1);
        strcpy(commandToExecute, token);

        token = strtok(NULL, ">>");
        char *fileName = malloc(strlen(token) + 1);
        strcpy(fileName, token);

        trim(commandToExecute);
        trim(fileName);

        int fd = open(fileName, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0) {
            perror("Error opening file");
            return;
        }

        int stdoutCopy = dup(STDOUT_FILENO);
        dup2(fd, STDOUT_FILENO);
        close(fd);

        executeCommand(commandToExecute, 0);

        dup2(stdoutCopy, STDOUT_FILENO);
        close(stdoutCopy);

        free(commandCopy);
        free(commandToExecute);
        free(fileName);
        return;
    }

    if (strchr(command, '>') != NULL) {
        writeFile(command);
        return;
    }
}
