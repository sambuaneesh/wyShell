#include "headers.h"
#include "tools.h"

void handleInput(char *input)
{
    Command *commandList = parseCommand(input, ";");

    for (int cmdIndex = 0; cmdIndex < commandList->argc; cmdIndex++)
    {
        Command *singleCommand = parseCommand(commandList->argv[cmdIndex], "&");

        // process commands seperated by &
        for (int subCmdIndex = 0; subCmdIndex < singleCommand->argc; subCmdIndex++)
        {
            // tokenizing current command
            Command *tokens = parseCommand(singleCommand->argv[subCmdIndex], " \t");
            if (strcmp(tokens->argv[0], "warp") == 0)
                warp(tokens);
            else if (strcmp(tokens->argv[0], "peek") == 0)
                peek(tokens);
            else if (strcmp(tokens->argv[0], "proclore") == 0)
                proclore(tokens);
            // else if (strcmp(tokens->argv[0], "seek") == 0)
            //     seek(tokens);
            else if (strcmp(tokens->argv[0], "pastevents") == 0)
                pastevents(tokens);
            else
            {
                executeCommand(tokens->argv);
            }

            freeCommand(tokens);
        }

        freeCommand(singleCommand);
    }

    freeCommand(commandList);
}
