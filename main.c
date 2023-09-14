
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
            printError("Terminated process with PID %d\n", foregroundPid);

            // Update the state of the foreground process to "Terminated"
            updateProcessState(foregroundPid, "Terminated");
        } else {
//            perror("Error sending SIGINT signal");
        }
    } else {
        // No foreground process is running
        printf("No foreground process running to terminate.\n");
    }
}

void handleCtrlZ(int signal) {
    if (process_list_head != NULL) {
        pid_t foregroundPid = process_list_head->process_info.pid;

        if (foregroundPid == getpid()) {
            // The foreground process is the shell itself, so Ctrl+Z has no effect
            printError("Ctrl+Z: No foreground process running to stop.\n");
        } else {
            // Send the SIGTSTP signal to the foreground process
            if (kill(foregroundPid, SIGTSTP) == 0) {
                // Print a message indicating Ctrl+Z was pressed
                printError("Ctrl+Z: Pushed running process with PID %d to background\n", foregroundPid);

                // Update the state of the foreground process to "Stopped"
                updateProcessState(foregroundPid, "Stopped");

                // Run the foreground process in the background
                if (setpgid(foregroundPid, foregroundPid) == 0) {
                    // Successfully moved to the background
                } else {
                    printError("Ctrl+Z: Error moving the process to the background");
                }
            } else {
                printError("Ctrl+Z: Error sending SIGTSTP signal");
            }
        }
    } else {
        // No foreground process running, so Ctrl+Z has no effect
        printError("Ctrl+Z: No foreground process running to stop.\n");
    }
}


int main() {
//    struct ProcessNode* process_list_head = NULL;
    global_home = initializeGlobalHome();
    signal(SIGINT, handleCtrlC);
    signal(SIGTSTP, handleCtrlZ);
//    signal(SIGQUIT, handleCtrlD);
    loadPastEvents();
    printWyshArt();

    int backgroundProcessStatus = 0;
    while (1) {
        prompt(backgroundProcessStatus);
        char input[MAX_INPUT_LENGTH];
//        fgets(input, MAX_INPUT_LENGTH, stdin);
        if (fgets(input, MAX_INPUT_LENGTH, stdin) == NULL) {
            // Handle Ctrl+D (EOF)
            printf("Ctrl+D received, logging out...\n");
            break; // Exit the shell
        }
        input[strcspn(input, "\n")] = '\0';

        addToPastEvents(input);
        printf(RESET_COLOR);

        if (strcmp(input, "exit") == 0) {
            for (int i = 0; i < num_past_events; i++) {
                free(past_events[i]);
            }
            return 0;
        } else {
            handleInput(input);
            backgroundProcessStatus = 0;
        }
//        viewProcesses();
    }
    freeProcessList();
    free(global_home);
    free(prev_directory);
    for (int i = 0; i < num_past_events; i++) {
        free(past_events[i]);
    }

    return 0;
}
