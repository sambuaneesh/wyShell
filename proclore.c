#include "headers.h"

void printProcessInfo(pid_t pid)
{
    char proc_path[1024];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/status", pid);

    FILE *status_file = fopen(proc_path, "r");
    if (status_file == NULL)
    {
        perror("Failed to open status file");
        return;
    }

    char line[256];
    char process_status[10] = "Unknown";
    char executable_path[1024] = "Unknown";

    while (fgets(line, sizeof(line), status_file) != NULL)
    {
        if (strncmp(line, "State:", 6) == 0)
        {
            sscanf(line, "State:\t%s", process_status);
        }
        else if (strncmp(line, "Name:", 5) == 0)
        {
            sscanf(line, "Name:\t%s", executable_path);
        }
    }

    fclose(status_file);

    printf("Process Information:\n");
    printf("PID: %d\n", pid);
    printf("Process Status: %s\n", process_status);
    printf("Executable Path: %s\n", executable_path);

    // You can include more information here, like Process Group and Virtual Memory.
}

void proclore(Command *cmd)
{
    if (cmd->argc == 1)
    {
        printProcessInfo(getpid()); // Print information about the shell process
    }
    else if (cmd->argc == 2)
    {
        pid_t pid;
        if (sscanf(cmd->argv[1], "%d", &pid) == 1)
        {
            printProcessInfo(pid);
        }
        else
        {
            printf("Invalid usage. Usage: proclore [pid]\n");
        }
    }
    else
    {
        printf("Invalid usage. Usage: proclore [pid]\n");
    }
}
