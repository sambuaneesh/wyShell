#include "headers.h"
#include "tools.h"
#include "handleInput.h"

char *global_home = NULL;
char *prev_directory = NULL;

int main()
{
    global_home = initializeGlobalHome();
    loadPastEvents();

    while (1)
    {
        prompt();
        char input[MAX_INPUT_LENGTH];
        fgets(input, MAX_INPUT_LENGTH, stdin);

        // replacing newline at the end
        input[strcspn(input, "\n")] = '\0';

        addToPastEvents(input);

        if (strcmp(input, "exit") == 0)
        {
            for (int i = 0; i < num_past_events; i++)
            {
                free(past_events[i]);
            }
            return 0;
        }
        else
            handleInput(input);
    }
}