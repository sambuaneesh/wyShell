#include "headers.h"

void search(const char *target, const char *directory, int isDirFlag, int isFileFlag, int isExecuteFlag, int topLevelSearch)
{
    DIR *dir = opendir(directory);
    if (dir == NULL)
    {
        perror("opendir");
        return;
    }

    int matchFound = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char entryPath[DEF_SIZE];
        snprintf(entryPath, sizeof(entryPath), "%s/%s", directory, entry->d_name);

        struct stat entryStat;
        if (stat(entryPath, &entryStat) == -1)
        {
            perror("stat");
            continue;
        }

        if (S_ISDIR(entryStat.st_mode) && (isDirFlag || (!isFileFlag && !isDirFlag)))
        {
            if (strcmp(entry->d_name, target) == 0 || strstr(entry->d_name, target) != NULL)
            {
                matchFound = 1;
                printf("%s/%s\n", directory, entry->d_name);
                if (isExecuteFlag)
                {
                    if (access(entryPath, X_OK) != 0)
                    {
                        perror("Missing permissions for task!");
                        closedir(dir);
                        return;
                    }
                    if (chdir(entryPath) == -1)
                    {
                        perror("chdir");
                        closedir(dir);
                        return;
                    }
                    printf("Changed current working directory to %s\n", entryPath);
                }
                search(target, entryPath, isDirFlag, isFileFlag, isExecuteFlag, 0);
            }
        }
        else if (S_ISREG(entryStat.st_mode) && (isFileFlag || (!isFileFlag && !isDirFlag)))
        {
            if (strstr(entry->d_name, target) != NULL)
            {
                matchFound = 1; // Mark match found
                printf("%s/%s\n", directory, entry->d_name);
                if (isExecuteFlag)
                {
                    if (access(entryPath, R_OK) != 0)
                    {
                        perror("Missing permissions for task!");
                        closedir(dir);
                        return;
                    }
                    FILE *file = fopen(entryPath, "r");
                    if (file == NULL)
                    {
                        perror("fopen");
                    }
                    else
                    {
                        char buffer[1024];
                        printf(DARK_GREY_COLOR);
                        while (fgets(buffer, sizeof(buffer), file) != NULL)
                        {
                            printf("%s", buffer);
                        }
                        printf(RESET_COLOR);
                        printf("\n");
                        fclose(file);
                    }
                }
            }
        }
    }
    closedir(dir);

    if (topLevelSearch && !matchFound && isDirFlag)
    {
        printf("No match found!\n");
    }
}

// Main seek function
void seek(Command *cmd)
{
    if (cmd->argc < 2)
    {
        printf("Usage: seek <flags> <search> <target_directory>\n");
        return;
    }

    int isDirFlag = 0;
    int isFileFlag = 0;
    int isExecuteFlag = 0;
    char *target = NULL;
    char *directory = "."; // Initialize with the current directory

    int hasInvalidFlags = 0;

    for (int i = 1; i < cmd->argc; i++)
    {
        if (strcmp(cmd->argv[i], "-d") == 0)
        {
            if (isFileFlag || isExecuteFlag)
            {
                hasInvalidFlags = 1;
                break;
            }
            isDirFlag = 1;
        }
        else if (strcmp(cmd->argv[i], "-f") == 0)
        {
            if (isDirFlag)
            {
                hasInvalidFlags = 1;
                break;
            }
            isFileFlag = 1;
        }
        else if (strcmp(cmd->argv[i], "-e") == 0)
        {
            if ((isDirFlag && isFileFlag) || isExecuteFlag)
            {
                hasInvalidFlags = 1;
                break;
            }
            isExecuteFlag = 1;
        }
        else if (target == NULL)
        {
            target = cmd->argv[i];
        }
        else if (i == cmd->argc - 1)
        {
            directory = cmd->argv[i];
        }
        else
        {
            printf("Invalid usage!\n");
            return;
        }
    }

    if (hasInvalidFlags)
    {
        printf("Invalid flags!\n");
        return;
    }

// Now, you can check for both flag combinations for displaying file contents
    if ((isFileFlag && isExecuteFlag) || (isExecuteFlag && isFileFlag))
    {
        search(target, directory, isDirFlag, isFileFlag, isExecuteFlag, 1); // Pass 1 for topLevelSearch
    }

    search(target, directory, isDirFlag, isFileFlag, isExecuteFlag, 1); // Pass 1 for topLevelSearch
}
