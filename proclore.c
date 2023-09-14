#include "headers.h"

void printProcessInfo(pid_t pid)
{
    char proc_path[1024];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/status", pid);

    FILE *status_file = fopen(proc_path, "r");
    if (status_file == NULL)
    {
        printError("Failed to open status file");
        return;
    }

    char line[256];
    char process_status[10] = "Unknown";
    char executable_path[1024] = "Unknown";
    int process_group = -1;
    int virtual_memory = -1;

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
        else if (strncmp(line, "VmSize:", 7) == 0)
        {
            sscanf(line, "VmSize:\t%d", &virtual_memory);
        }
        else if (strncmp(line, "Pid:", 4) == 0)
        {
            sscanf(line, "Pid:\t%d", &process_group);
        }
    }

    fclose(status_file);

    char status_sign = (strstr(process_status, "T") == NULL) ? '+' : ' ';

    printf("pid : %d\n", pid);
    printf("process status : %s%c\n", process_status, status_sign);
    printf("Process Group : %d\n", process_group);
    printf("Virtual memory : %d\n", virtual_memory);
    printf("executable path : %s\n", executable_path);
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
            printError("Invalid usage. Usage: proclore [pid]\n");
        }
    }
    else
    {
        printError("Invalid usage. Usage: proclore [pid]\n");
    }
}
