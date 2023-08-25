#include "headers.h"

Command *parseCommand(char *command, const char *delimiter)
{
    int numTokens = 0;
    char *tokens[MAX_INPUT_LENGTH];
    char *token = strtok(command, delimiter);
    while (token != NULL)
    {
        tokens[numTokens] = strdup(token);
        numTokens++;
        token = strtok(NULL, delimiter);
    }

    Command *cmd = malloc(sizeof(Command));
    cmd->argc = numTokens;
    cmd->argv = malloc((numTokens + 1) * sizeof(char *));
    for (int i = 0; i < numTokens; i++)
    {
        cmd->argv[i] = strdup(tokens[i]);
    }
    cmd->argv[numTokens] = NULL;

    return cmd;
}

void freeCommand(Command *cmd)
{
    if (cmd == NULL)
    {
        return;
    }

    for (int i = 0; i < cmd->argc; i++)
    {
        free(cmd->argv[i]);
    }
    free(cmd->argv);
    free(cmd);
}