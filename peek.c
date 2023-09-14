#include "headers.h"
#include "tools.h"

int compare(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

typedef struct
{
    const char *directory;
    int showHidden;
    int showDetails;
} ListOptions;

void applyColor(struct stat file_stat)
{
    if (S_ISDIR(file_stat.st_mode))
    {
        printf(BLUE_COLOR);
    }
    else if (file_stat.st_mode & S_IXUSR)
    {
        printf(GREEN_COLOR);
    }
}

void listWithOptions(ListOptions options)
{
    DIR *dir = opendir(options.directory);
    if (dir == NULL)
    {
        printError("opendir error");
        return;
    }

    struct dirent *entry;
    size_t num_entries = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        if (options.showHidden || entry->d_name[0] != '.')
        {
            num_entries++;
        }
    }

    rewinddir(dir);

    char **entries = (char **)malloc(num_entries * sizeof(char *));
    if (entries == NULL)
    {
        printError("memory allocation error");
        closedir(dir);
        return;
    }

    size_t idx = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        if (options.showHidden || entry->d_name[0] != '.')
        {
            entries[idx] = strdup(entry->d_name);
            idx++;
        }
    }

    qsort(entries, num_entries, sizeof(char *), compare);

    for (size_t i = 0; i < num_entries; i++)
    {
        struct stat file_stat;
        char filepath[DEF_SIZE];
        snprintf(filepath, sizeof(filepath), "%s/%s", options.directory, entries[i]);

        if (stat(filepath, &file_stat) == 0)
        {
            applyColor(file_stat);
        }

        printf("%s%s\n", entries[i], RESET_COLOR);
        free(entries[i]);
    }

    free(entries);
    closedir(dir);
}

void listDirectoryHidden(const char *directory)
{
    ListOptions options = {directory, 1, 0};
    listWithOptions(options);
}

void listDirectory(const char *directory)
{
    ListOptions options = {directory, 0, 0};
    listWithOptions(options);
}

void listDetailedDirectory(const char *directory)
{
    DIR *dir = opendir(directory);
    if (dir == NULL)
    {
        printError("opendir error");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        char *filename = entry->d_name;
        char filepath[DEF_SIZE];
        snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);

        struct stat file_stat;
        if (stat(filepath, &file_stat) < 0)
        {
            printError("stat error");
            closedir(dir);
            return;
        }

        // File type and permissions
        printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
        printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
        printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
        printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
        printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
        printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
        printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
        printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
        printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");
        printf(RESET_COLOR); // Reset color
        printf(" ");

        // Other details
        printf("%ld ", file_stat.st_nlink);

        struct passwd *pw = getpwuid(file_stat.st_uid);
        if (pw == NULL)
        {
            printError("getpwuid error");
            closedir(dir);
            return;
        }
        printf("%s ", pw->pw_name);

        struct group *gr = getgrgid(file_stat.st_gid);
        if (gr == NULL)
        {
            printError("getgrgid error");
            closedir(dir);
            return;
        }
        printf("%s ", gr->gr_name);

        printf("%6ld ", file_stat.st_size);

        // Format time
        struct tm *timeinfo = localtime(&file_stat.st_mtime);
        char time_buffer[80];
        strftime(time_buffer, sizeof(time_buffer), "%b %d %H:%M", timeinfo);
        printf("%s ", time_buffer);

        // File name with color for directories and executables
        if (S_ISDIR(file_stat.st_mode))
        {
            printf(BLUE_COLOR "%s" RESET_COLOR, filename);
        }
        else if (file_stat.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))
        {
            printf(GREEN_COLOR "%s" RESET_COLOR, filename);
        }
        else
        {
            printf("%s", filename);
        }

        printf("\n");
    }

    if (closedir(dir) != 0)
    {
        printError("closedir error");
    }
}
void peek(Command *cmd)
{
    int showAll = 0;
    int showDetails = 0;
    char *directory = NULL;

    for (int i = 1; i < cmd->argc; i++)
    {
        if (strcmp(cmd->argv[i], "-a") == 0)
        {
            showAll = 1;
        }
        else if (strcmp(cmd->argv[i], "-l") == 0 || strcmp(cmd->argv[i], "-la") == 0 || strcmp(cmd->argv[i], "-al") == 0)
        {
            showDetails = 1;
        }
        else
        {
            directory = cmd->argv[i];
        }
    }

    if (directory == NULL)
    {
        // peek at current working directory
        char current_directory[DEF_SIZE];
        if (getcwd(current_directory, sizeof(current_directory)) == NULL)
        {
            printError("getcwd error");
            return;
        }

        if (showDetails)
            listDetailedDirectory(current_directory);
        else if (showAll)
            listDirectoryHidden(current_directory);
        else
            listDirectory(current_directory);
    }
    else
    {
        if (directory[0] == '~' && (directory[2] == '/' || directory[2] == '\0'))
        {
            // handling ~/dir case
            char expanded_path[DEF_SIZE];
            snprintf(expanded_path, sizeof(expanded_path), "%s%s", global_home, directory + 1);
            directory = expanded_path;
        }

        if (showDetails)
            listDetailedDirectory(directory);
        else if (showAll)
            listDirectoryHidden(directory);
        else
            listDirectory(directory);
    }
}
