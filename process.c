#include "headers.h"

// Function to insert a process into the linked list in lexicographic order
void insertProcess(ProcessInfo new_process) {
    struct ProcessNode *new_node = malloc(sizeof(struct ProcessNode));
    if (new_node == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    new_node->process_info = new_process;
    new_node->next = NULL;
    snprintf(new_node->process_info.state, sizeof(new_node->process_info.state), "Running");

    // If the list is empty or the new process should be the first,
    // update the head pointer
    if (process_list_head == NULL || new_process.pid < process_list_head->process_info.pid) {
        new_node->next = process_list_head;
        process_list_head = new_node;
    } else {
        struct ProcessNode *current = process_list_head;
        while (current->next != NULL && new_process.pid > current->next->process_info.pid) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}


// Function to handle the SIGCHLD signal
void sigchld_handler() {
    int status;
    pid_t child_pid;

    while ((child_pid = waitpid(-1, &status, WNOHANG)) > 0) {
        // Check if the child process exited successfully
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("\tProcess with PID:(%d) exited normally\n", child_pid);
            prompt(0);
            fflush(stdout);
            struct ProcessNode *current = process_list_head;
            while (current != NULL) {
                if (current->process_info.pid == child_pid) {
                    snprintf(current->process_info.state, sizeof(current->process_info.state), "Stopped");
                    break; // Found and updated, exit loop
                }
                current = current->next;
            }
        }
    }
}

