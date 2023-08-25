#include "headers.h"
#include "tools.h"

void executeCommand(char *tokens[])
{
    pid_t pid = fork();

    if (pid == 0)
    {
        // Child process
        execvp(tokens[0], tokens);
        // perror("Command execution error");
        fprintf(stderr, "ERROR : '%s' is not a valid command\n", tokens[0]);
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        // Parent process
        waitpid(pid, NULL, 0);
    }
    else
    {
        perror("Fork error");
    }
}