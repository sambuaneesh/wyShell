#include "headers.h"
#include "tools.h"
#include "handleInput.h"

char *global_home = NULL;
char *prev_directory = NULL;

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

        if (strcmp(input, "exit") == 0)
            return 0;
        else
            handleInput(input);
    }
}