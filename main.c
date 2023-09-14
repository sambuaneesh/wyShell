#include "headers.h"
#include "tools.h"
#include "handleInput.h"

char *global_home = NULL;
char *prev_directory = NULL;

struct ProcessNode *process_list_head = NULL;


void handleCtrlC(int signal) {
    if (process_list_head != NULL) {
        // There is a foreground process running
        pid_t foregroundPid = process_list_head->process_info.pid;

        // Send the SIGINT signal to the foreground process
        if (kill(foregroundPid, SIGINT) == 0) {
            // Print a message indicating Ctrl+C was pressed
            printf("Terminated process with PID %d\n", foregroundPid);

            // Update the state of the foreground process to "Terminated"
            updateProcessState(foregroundPid, "Terminated");
        } else {
            perror("Error sending SIGINT signal");
        }
    } else {
        // No foreground process is running
        printf("No foreground process running to terminate.\n");
    }
}


int main()
{
//    struct ProcessNode* process_list_head = NULL;
    global_home = initializeGlobalHome();
    loadPastEvents();
    printWyshArt();

    signal(SIGINT, handleCtrlC);
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
