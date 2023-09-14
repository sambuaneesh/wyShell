#include "headers.h"
#include "tools.h"

void bringBackgroundToForeground(int pid) {
    struct ProcessNode *current = process_list_head;

    while (current != NULL) {
        if (current->process_info.pid == pid) {
            // Execute the command associated with the process
            executeCommand(current->process_info.complete_command, 0);
            copyProcessInfoToLatest(pid);
            return;
        }
        current = current->next;
    }

    printf("No such process found.\n");
}

void changeBackgroundProcessState(int pid) {
    struct ProcessNode *current = process_list_head;

    while (current != NULL) {
        if (current->process_info.pid == pid) {
            executeCommand(current->process_info.complete_command, 1);
            return;
        }
        current = current->next;
    }

    printError("No such process found.\n");
}


void handleFgBgCommand(Command *cmd) {
    if (cmd->argc < 2) {
        printError("Invalid command format. Usage: fg <pid> or bg <pid>\n");
        return;
    }

    int pid;
    if (sscanf(cmd->argv[1], "%d", &pid) == 1) {
        if (strcmp(cmd->argv[0], "fg") == 0) {
            bringBackgroundToForeground(pid);
        }
        else if (strcmp(cmd->argv[0], "bg") == 0) {
            changeBackgroundProcessState(pid);
        } else {
            printError("Invalid command format. Usage: fg <pid> or bg <pid>\n");
        }
    } else {
        printError("Invalid PID format. Usage: fg <pid> or bg <pid>\n");
    }
}

