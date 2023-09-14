#include "headers.h"
#include "tools.h"
void handleInput(char *input)
{
    Command *commandList = parseCommand(input, ";");

    for (int cmdIndex = 0; cmdIndex < commandList->argc; cmdIndex++)
    {
        char *command = commandList->argv[cmdIndex];
        char *ampersand = strchr(command, '&');

        if (ampersand != NULL)
        {
            *ampersand = '\0';
            char *nextCommand = ampersand + 1;

            if (*nextCommand == ' ')
                nextCommand++;

            executeCommand(command, 1);

            handleInput(nextCommand);
        }
        else
        {
            executeCommand(command, 0);
        }
    }

    freeCommand(commandList);
}
