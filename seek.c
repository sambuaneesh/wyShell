#include "headers.h"
#include <errno.h>

/*Specification : seek
‘seek’ command looks for a file/directory in the specified target directory (or current if no directory is specified). It returns a list of relative paths (from target directory) of all matching files/directories (files in green and directories in blue) separated with a newline character.

Note that by files, the text here refers to non-directory files.

Flags :

-d : Only look for directories (ignore files even if name matches)
-f : Only look for files (ignore directories even if name matches)
-e : This flag is effective only when a single file or a single directory with the name is found. If only one file (and no directories) is found, then print it’s output. If only one directory (and no files) is found, then change current working directory to it. Otherwise, the flag has no effect. This flag should work with -d and -f flags. If -e flag is enabled but the directory does not have access permission (execute) or file does not have read permission, then output “Missing permissions for task!”*/

// Function to search for files/directories
void search(const char *target, const char *directory, int isDirFlag, int isFileFlag, int isExecuteFlag)
{
    DIR *dir = opendir(directory);
    if (dir == NULL)
    {
        perror("opendir");
        return;
    }

    int matchFound = 0; // Flag to track if any matches were found
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char entryPath[PATH_MAX];
        snprintf(entryPath, sizeof(entryPath), "%s/%s", directory, entry->d_name);

        struct stat entryStat;
        if (stat(entryPath, &entryStat) == -1)
        {
            perror("stat");
            continue;
        }

        if (S_ISDIR(entryStat.st_mode) && (isDirFlag || (!isFileFlag && !isDirFlag)))
        {
            if (strstr(entry->d_name, target) != NULL)
            {
                matchFound = 1; // Mark match found
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
                search(target, entryPath, isDirFlag, isFileFlag, isExecuteFlag);
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
                        while (fgets(buffer, sizeof(buffer), file) != NULL)
                        {
                            printf("%s", buffer);
                        }
                        printf("\n");
                        fclose(file);
                    }
                }
            }
        }
    }
    closedir(dir);

    if (!matchFound)
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
    char *directory = "."; // Initialize with current directory

    for (int i = 1; i < cmd->argc; i++)
    {
        if (strcmp(cmd->argv[i], "-d") == 0)
        {
            isDirFlag = 1;
        }
        else if (strcmp(cmd->argv[i], "-f") == 0)
        {
            isFileFlag = 1;
        }
        else if (strcmp(cmd->argv[i], "-e") == 0)
        {
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

    search(target, directory, isDirFlag, isFileFlag, isExecuteFlag);
}
