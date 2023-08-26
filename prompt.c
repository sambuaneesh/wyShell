#include "headers.h"

char *getUsername()
{
    struct passwd *pw;
    uid_t uid = getuid();
    pw = getpwuid(uid);
    if (!pw)
    {
        fprintf(stderr, "Cannot get username");
        exit(EXIT_FAILURE);
    }
    return pw->pw_name;
}

void getHostname(char *hostname)
{
    if (gethostname(hostname, DEF_SIZE))
    {
        fprintf(stderr, "Cannot get hostname");
        exit(EXIT_FAILURE);
    }
}

void getCurrentDirectory(char *cwd)
{
    if (!getcwd(cwd, DEF_SIZE))
    {
        fprintf(stderr, "Cannot get current working directory");
    }
}

void printPrompt(const char *username, const char *hostname, const char *cwd, int backgroundProcessStatus)
{
    printf(BOLD_CYAN_COLOR);

    // if cwd is @home
    if (strcmp(cwd, global_home) == 0)
    {
        printf("<%s%s@%s%s:~%s", BOLD_YELLOW_COLOR, username, BOLD_MAGENTA_COLOR, hostname, RESET_COLOR);
    }
    // if cwd is @home/..
    else if (strstr(cwd, global_home) == cwd)
    {
        printf("<%s%s@%s%s:~%s%s", BOLD_YELLOW_COLOR, username, BOLD_MAGENTA_COLOR, hostname, BOLD_GREY_COLOR, cwd + strlen(global_home));
    }
    // if cwd is not @home
    else
    {
        printf("<%s%s@%s%s:%s%s", BOLD_YELLOW_COLOR, username, BOLD_MAGENTA_COLOR, hostname, BOLD_GREY_COLOR, cwd);
    }

    // Print background process status and time taken
    if (backgroundProcessStatus > 0)
    {
        printf(BOLD_CYAN_COLOR " (Process %d)", backgroundProcessStatus);
    }

    printf(BOLD_CYAN_COLOR ">%s ", RESET_COLOR);
}

void prompt(int backgroundProcessStatus)
{
    char cwd[DEF_SIZE];

    getCurrentDirectory(cwd);

    char *username = getUsername();

    char hostname[DEF_SIZE];
    getHostname(hostname);

    printPrompt(username, hostname, cwd, backgroundProcessStatus);
}
