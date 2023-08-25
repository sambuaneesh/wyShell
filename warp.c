#include "headers.h"

void warpHelper(char *directory)
{
    if (directory == NULL)
    {
        fprintf(stderr, "warp: missing argument\n");
        return;
    }

    char current_directory[DEF_SIZE];
    if (getcwd(current_directory, sizeof(current_directory)) == NULL)
    {
        perror("getcwd error");
        return;
    }

    if (strcmp(directory, "-") == 0)
    {
        if (prev_directory == NULL)
        {
            fprintf(stderr, "warp: no previous directory\n");
            return;
        }
        directory = prev_directory;
    }
    else if (directory[0] == '~' && (directory[1] == '/' || directory[1] == '\0'))
    {
        // handling ~/dir case
        char expanded_path[DEF_SIZE];
        snprintf(expanded_path, sizeof(expanded_path), "%s%s", global_home, directory + 1);
        directory = expanded_path;
    }

    if (chdir(directory) != 0)
    {
        perror("warp error");
    }
    else
    {
        prev_directory = strdup(current_directory);
        if (prev_directory == NULL)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
    }
    char working_directory[DEF_SIZE];
    if (getcwd(working_directory, sizeof(working_directory)) == NULL)
    {
        perror("getcwd error");
        return;
    }
    printf("%s\n", working_directory);
}

void warp(Command *cmd)
{
    if (cmd->argc <= 1)
    {
        fprintf(stderr, "warp: missing argument\n");
        return;
    }

    for (int i = 1; i < cmd->argc; i++)
    {
        warpHelper(cmd->argv[i]);
    }
}