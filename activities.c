#include "headers.h"
#include "tools.h"

void viewProcesses() {
    struct ProcessNode *current = process_list_head;

    if (current == NULL) {
        printf("No processes to display.\n");
        return;
    }

    printf("Processes in lexicographic order:\n");

    while (current != NULL) {
        printf("[%d] : %s - %s\n", current->process_info.pid, current->process_info.command,
               current->process_info.is_background ? "Background" : "Foreground");
        current = current->next;
    }
}


// Function to print activities
void printActivities() {
    struct ProcessNode *current = process_list_head;

//    printf("Activities:\n");

    while (current != NULL) {
        printf("[%d] : %s - %s\n", current->process_info.pid, current->process_info.command,
               current->process_info.state);
        current = current->next;
    }

    if (process_list_head == NULL) {
        printf("No processes in the activities list.\n");
    }
}

