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

        Command *inp = parseCommand(input, ";");

        // printf("Command %d:\n", i + 1);
        for (int j = 0; j < inp->argc; j++)
        {
            // printf("  Argument %d: %s\n", j + 1, cmd->argv[j]);
            printf("Command %d:\n", j + 1);
            Command *cmd = parseCommand(inp->argv[j], "&");
            for (int i = 0; i < cmd->argc; i++)
            {
                Command *tokens = parseCommand(cmd->argv[i], " \t");
                printf("\tTokens %d:\n\t\t|", i + 1);
                for (int k = 0; k < tokens->argc; k++)
                {
                    printf("%s|", tokens->argv[k]);
                }
                printf("\n");
            }
        }

        freeCommand(inp);
    }

    return 0;
}
