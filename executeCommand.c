#include "headers.h"
#include "tools.h"

void executeCommand(char *command, int isBackground) {
    signal(SIGCHLD, sigchld_handler);
    int hasPipe = 0;
    int hasIO = 0;

    // Check if there is a pipe
    if (strchr(command, '|') != NULL) {
        hasPipe = 1;
    }

    // Check if there is IO redirection
    if (strchr(command, '<') != NULL || strchr(command, '>') != NULL) {
        hasIO = 1;
    }

    if (hasPipe && hasIO) {
        ioPipe(command);
        return;
    } else if (hasPipe) {
        pipeCommand(command);
        return;
    } else if (hasIO) {
        ioRedirection(command);
        return;
    }

    Command *cmd = parseCommand(command, " \t");

    // Build the complete command string by concatenating all arguments
    char completeCommand[512]; // Adjust the size as needed
    completeCommand[0] = '\0'; // Initialize as an empty string
    for (int i = 0; i < cmd->argc; i++) {
        strcat(completeCommand, cmd->argv[i]);
        if (i < cmd->argc - 1) {
            strcat(completeCommand, " "); // Add a space between arguments
        }
    }

    if (strcmp(cmd->argv[0], "warp") == 0) {
        warp(cmd);
    } else if (strcmp(cmd->argv[0], "proclore") == 0) {
        proclore(cmd);
    } else if (strcmp(cmd->argv[0], "peek") == 0) {
        peek(cmd);
    } else if (strcmp(cmd->argv[0], "seek") == 0) {
        seek(cmd);
    } else if (strcmp(cmd->argv[0], "pastevents") == 0) {
        pastevents(cmd);
    } else if (strcmp(cmd->argv[0], "iMan") == 0) {
        iMan(cmd);
    } else if (strcmp(cmd->argv[0], "activities") == 0) {
        printActivities();
    } else if (strcmp(cmd->argv[0], "fg") == 0 || strcmp(cmd->argv[0], "bg") == 0) {
        handleFgBgCommand(cmd);
    } else if (strcmp(cmd->argv[0], "ping") == 0) {
        if (cmd->argc < 3) {
            printf("Invalid command format. Usage: ping <pid> <signal_number>\n");
        } else {
            int pid, signal_number;
            if (sscanf(cmd->argv[1], "%d", &pid) == 1 && sscanf(cmd->argv[2], "%d", &signal_number) == 1) {
                // Check if the process with the given PID exists
                if (processExists(pid)) {
                    // Calculate the actual signal number based on modulo 32
                    signal_number %= 32;
                    // Send the specified signal to the process
                    if (kill(pid, signal_number) == 0) {
                        printf("Sent signal %d to PID %d\n", signal_number, pid);

                        // Update the state of the process to "Stopped" if the signal is SIGSTOP
                        if (signal_number == SIGSTOP) {
                            updateProcessState(pid, "Stopped");
                        } else if (signal_number == SIGKILL) {
                            updateProcessState(pid, "Stopped");
                        }
                    } else {
                        perror("Error sending signal");
                    }
                } else {
                    printf("No such process found.\n");
                }
            } else {
                printf("Invalid PID or signal number format.\n");
            }
        }
    } else {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            if (execvp(cmd->argv[0], cmd->argv) == -1) {
                fprintf(stderr, "ERROR: '%s' is not a valid command\n", cmd->argv[0]);
                exit(EXIT_FAILURE);
            }
        } else if (pid > 0) {
            // Parent process
            ProcessInfo process_info;
            process_info.pid = pid;
            snprintf(process_info.command, sizeof(process_info.command), "%s", cmd->argv[0]);
            snprintf(process_info.complete_command, sizeof(process_info.complete_command), "%s",
                     completeCommand); // Store complete command
            process_info.is_background = isBackground;

            // Insert process info into the list for both foreground and background
            insertProcess(process_info);

            if (!isBackground) {
                // Foreground process
                int status;
                struct timeval start_time, end_time;
                gettimeofday(&start_time, NULL);
                waitpid(pid, &status, 0);
                gettimeofday(&end_time, NULL);

                // Calculate time in milliseconds
                long elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000 +
                                    (end_time.tv_usec - start_time.tv_usec) / 1000;

                if (elapsed_time > 2000) { // If time taken > 2 seconds
                    printf("(%s : %lds) ", cmd->argv[0], elapsed_time / 1000);
                }

                // Search for the foreground process by PID and update its state
                struct ProcessNode *current = process_list_head;
                while (current != NULL) {
                    if (current->process_info.pid == pid) {
                        snprintf(current->process_info.state, sizeof(current->process_info.state), "Stopped");
                        break; // Found and updated, exit loop
                    }
                    current = current->next;
                }
            } else {
                // Background process
                printf("[%d]\n", pid);
                fflush(stdout);
            }
        } else {
            printf("Error forking!\n");
        }
    }
    freeCommand(cmd);
}
