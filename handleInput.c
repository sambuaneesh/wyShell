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
            // Split the command at the first "&"
            *ampersand = '\0';
            char *nextCommand = ampersand + 1;

            if (*nextCommand == ' ')
                nextCommand++;

            // Execute the first part as a background process
            executeCommand(command, 1);

            // Handle the remaining part recursively
            handleInput(nextCommand);
        }
        else
        {
            // No "&" found, execute the command as foreground
            executeCommand(command, 0);
        }
    }

    freeCommand(commandList);
}
