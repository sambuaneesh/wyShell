#include "headers.h"
#include "tools.h"
#include "handleInput.h"

char *global_home = NULL;
char *prev_directory = NULL;

struct ProcessNode *process_list_head = NULL;

int main()
{
//    struct ProcessNode* process_list_head = NULL;
    global_home = initializeGlobalHome();
    loadPastEvents();
    printWyshArt();

    int backgroundProcessStatus = 0;

    while (1)
    {
        prompt(backgroundProcessStatus);

        char input[MAX_INPUT_LENGTH];
        fgets(input, MAX_INPUT_LENGTH, stdin);

        input[strcspn(input, "\n")] = '\0';

        addToPastEvents(input);
        printf(RESET_COLOR);

        if (strcmp(input, "exit") == 0)
        {
            for (int i = 0; i < num_past_events; i++)
            {
                free(past_events[i]);
            }
            return 0;
        }
        else
        {
            handleInput(input);
            backgroundProcessStatus = 0;
        }
//        viewProcesses();
    }
}
