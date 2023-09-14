#include "headers.h"
#include "tools.h"

// Function to bring a background process to the foreground and resume it
void bringBackgroundToForeground(int pid) {
    struct ProcessNode *current = process_list_head;

    while (current != NULL) {
        if (current->process_info.pid == pid) {
            // Change the state of the process to Running (Foreground)
//            snprintf(current->process_info.state, sizeof(current->process_info.state), "Running");
//
//            // Set is_background to 0 for foreground process
//            current->process_info.is_background = 0;
//
//            // Move the process to the foreground by giving it control of the terminal
//            // You can use tcsetpgrp to do this, but be sure to save and restore the terminal's
//            // foreground process group when the process exits.
//
//            // Print a message indicating success
//            printf("[%d] %s - Running (Foreground)\n", current->process_info.pid, current->process_info.command);

            // Execute the command associated with the process
            executeCommand(current->process_info.complete_command, 0);
//            printf("%d",deleteLatestProcess());
            copyProcessInfoToLatest(pid);
            return;
        }
        current = current->next;
    }

    // If the process with the given PID was not found
    printf("No such process found.\n");
}

// Function to change the state of a stopped background process to Running (bg) and resume it
void changeBackgroundProcessState(int pid) {
    struct ProcessNode *current = process_list_head;

    while (current != NULL) {
        if (current->process_info.pid == pid) {
//            // Change the state of the background process to Running
//            snprintf(current->process_info.state, sizeof(current->process_info.state), "Running");
//
//            // Set is_background to 1 for background process
//            current->process_info.is_background = 1;
//
//            // Implement logic to resume a stopped process using signals (e.g., SIGCONT).
//
//            // Print a message indicating success
//            printf("[%d] %s - Running (Background)\n", current->process_info.pid, current->process_info.command);

            // Execute the command associated with the process
            executeCommand(current->process_info.complete_command, 1);
            return;
        }
        current = current->next;
    }

    // If the process with the given PID was not found
    printf("No such process found.\n");
}


// Function to handle the 'fg' and 'bg' commands
void handleFgBgCommand(Command *cmd) {
    // Ensure that there is a second argument (PID) for the command
    if (cmd->argc < 2) {
        printf("Invalid command format. Usage: fg <pid> or bg <pid>\n");
        return;
    }

    int pid;
    if (sscanf(cmd->argv[1], "%d", &pid) == 1) {
        // Check if the command is "fg"
        if (strcmp(cmd->argv[0], "fg") == 0) {
            bringBackgroundToForeground(pid);
        }
            // Check if the command is "bg"
        else if (strcmp(cmd->argv[0], "bg") == 0) {
            changeBackgroundProcessState(pid);
        } else {
            // Invalid command format
            printf("Invalid command format. Usage: fg <pid> or bg <pid>\n");
        }
    } else {
        // Invalid PID
        printf("Invalid PID format. Usage: fg <pid> or bg <pid>\n");
    }
}

