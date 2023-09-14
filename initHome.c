#include "headers.h"

char *initializeGlobalHome()
{
    char *cwd = malloc(DEF_SIZE);
    if (cwd == NULL)
    {
        printError("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    if (getcwd(cwd, DEF_SIZE) == NULL)
    {
        printError("Error getting current directory");
        free(cwd);
        exit(EXIT_FAILURE);
    }

    return strdup(cwd);
}