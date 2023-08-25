#include "headers.h"
#include "tools.h"

char *global_home = NULL;

int main()
{
    global_home = initializeGlobalHome();

    while (1)
    {
        prompt();
        char input[MAX_INPUT_LENGTH];
        fgets(input, MAX_INPUT_LENGTH, stdin);

        // replacing newline at the end
        input[strcspn(input, "\n")] = '\0';

        Command *commandList = parseCommand(input, ";");

        for (int cmdIndex = 0; cmdIndex < commandList->argc; cmdIndex++)
        {
            Command *singleCommand = parseCommand(commandList->argv[cmdIndex], "&");

            // process commands seperated by &
            for (int subCmdIndex = 0; subCmdIndex < singleCommand->argc; subCmdIndex++)
            {
                // tokenizing current command
                Command *tokens = parseCommand(singleCommand->argv[subCmdIndex], " \t");

                freeCommand(tokens);
            }

            freeCommand(singleCommand);
        }

        freeCommand(commandList);
    }

    return 0;
}
