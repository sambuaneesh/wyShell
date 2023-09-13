#include "headers.h"
#include "tools.h"


void executeCommand(char *command, int isBackground)
{
    signal(SIGCHLD, sigchld_handler);
    if (strchr(command, '<') != NULL || strchr(command, '>')) {
        ioRedirection(command);
        return;
    }
    if (pipeCommand(command))
        return;
    Command *cmd = parseCommand(command, " \t");
    if (strcmp(cmd->argv[0], "warp") == 0)
    {
        warp(cmd);
    }
    else if (strcmp(cmd->argv[0], "proclore") == 0)
    {
        proclore(cmd);
    }
    else if (strcmp(cmd->argv[0], "peek") == 0)
    {
        peek(cmd);
    }
    else if (strcmp(cmd->argv[0], "seek") == 0)
    {
        seek(cmd);
    }
    else if (strcmp(cmd->argv[0], "pastevents") == 0)
    {
        pastevents(cmd);
    } else if (strcmp(cmd->argv[0], "iMan") == 0) {
        iMan(cmd);
    } else if (strcmp(cmd->argv[0], "activities") == 0) {
        printActivities();
    }
    else
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            // Child process
            if (execvp(cmd->argv[0], cmd->argv) == -1)
            {
                // printf("Invalid command!\n");
                fprintf(stderr, "ERROR: '%s' is not a valid command\n", cmd->argv[0]);
                exit(EXIT_FAILURE);
            }
        }
        else if (pid > 0)
        {
            // Parent process
            ProcessInfo process_info;
            process_info.pid = pid;
            snprintf(process_info.command, sizeof(process_info.command), "%s", cmd->argv[0]);
            process_info.is_background = isBackground;

            // Insert process info into the list for both foreground and background
            insertProcess(process_info);

            if (!isBackground)
            {
                // Foreground process
                int status;
                struct timeval start_time, end_time;
                gettimeofday(&start_time, NULL);
                waitpid(pid, &status, 0);
                gettimeofday(&end_time, NULL);

                // Calculate time in milliseconds
                long elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000 +
                                    (end_time.tv_usec - start_time.tv_usec) / 1000;

                if (elapsed_time > 2000) // If time taken > 2 seconds
                {
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
            }
            else
            {
                // Background process
                printf("[%d]\n", pid);
                fflush(stdout);
            }
        }
        else
        {
            printf("Error forking!\n");
        }
    }
    freeCommand(cmd);
}
