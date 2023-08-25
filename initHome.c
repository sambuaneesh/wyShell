#include "headers.h"

char *initializeGlobalHome()
{
    char *cwd = malloc(DEF_SIZE);
    if (cwd == NULL)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    if (getcwd(cwd, DEF_SIZE) == NULL)
    {
        perror("Error getting current directory");
        free(cwd);
        exit(EXIT_FAILURE);
    }

    return strdup(cwd);
}