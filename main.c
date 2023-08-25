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

        // Remove newline character from input
        input[strcspn(input, "\n")] = '\0';

        Command *cmd = parseCommand(input);

        // printf("Command %d:\n", i + 1);
        for (int j = 0; j < cmd->argc; j++)
        {
            printf("  Argument %d: %s\n", j + 1, cmd->argv[j]);
        }

        // Free allocated memory for commands
        freeCommand(cmd);
    }

    return 0;
}
