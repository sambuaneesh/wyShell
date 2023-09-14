#include "headers.h"
#include "tools.h"

int processExists(int pid) {
    struct ProcessNode *current = process_list_head;
    while (current != NULL) {
        if (current->process_info.pid == pid) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void updateProcessState(int pid, const char *state) {
    struct ProcessNode *current = process_list_head;
    while (current != NULL) {
        if (current->process_info.pid == pid) {
            strncpy(current->process_info.state, state, sizeof(current->process_info.state) - 1);
            current->process_info.state[sizeof(current->process_info.state) - 1] = '\0'; // Ensure null-termination
            break; // Found and updated, exit loop
        }
        current = current->next;
    }
}


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

// Function to delete the latest process from the process list and return its PID
pid_t deleteLatestProcess() {
    if (process_list_head == NULL) {
        printf("Process list is empty.\n");
        return -1; // Return an invalid PID
    }

    struct ProcessNode *current = process_list_head;
    struct ProcessNode *prev = NULL;

    // Traverse the list to find the latest process (the one at the end)
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }

    pid_t latest_pid = current->process_info.pid;

    // Delete the latest process from the list
    if (prev != NULL) {
        prev->next = NULL;
    } else {
        // If there was only one process in the list, update the head pointer
        process_list_head = NULL;
    }

    // Free the memory occupied by the deleted node
    free(current);

    return latest_pid;
}

// Function to copy the contents of the latest process to the specified process
#include "headers.h"  // Include your header file that defines ProcessNode and ProcessInfo

void copyProcessInfoToLatest(pid_t pid) {
    struct ProcessNode *latest = process_list_head;

    while (latest != NULL) {
        if (latest->process_info.pid == pid) {
            // Copy data from the latest process to the specified PID process
            ProcessInfo *latest_info = &(latest->process_info);
            // Find the specified PID process (excluding PID)
            struct ProcessNode *current = process_list_head;
            while (current != NULL) {
                if (current->process_info.pid != pid) {
                    // Copy data except for PID
                    current->process_info.is_background = latest_info->is_background;
                    current->process_info.exit_status = latest_info->exit_status;
                    strncpy(current->process_info.state, latest_info->state, sizeof(current->process_info.state));
                    strncpy(current->process_info.command, latest_info->command, sizeof(current->process_info.command));
                    strncpy(current->process_info.complete_command, latest_info->complete_command,
                            sizeof(current->process_info.complete_command));
                }
                current = current->next;
            }
            break;  // Exit loop after copying
        }
        latest = latest->next;
    }
    deleteLatestProcess();
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

