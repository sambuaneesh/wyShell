#ifndef HEADERS_H_
#define HEADERS_H_

// default buffer size
#define DEF_SIZE 1024
#define MAX_INPUT_LENGTH 4096
#define MAX_PAST_EVENTS 15
#define MAX_PATH_LENGTH 4096
#define PAST_EVENTS_FILE "pastevents.txt"

#define RESET_COLOR "\033[0m"
#define BLUE_COLOR "\033[34m"
#define GREEN_COLOR "\033[32m"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pwd.h>
#include <limits.h>
#include <dirent.h>
#include <grp.h>
#include <time.h>

extern char *global_home;
extern char *prev_directory;

extern char *past_events[MAX_PAST_EVENTS];
extern int num_past_events;

typedef struct
{
    char **argv;
    int argc;
} Command;

typedef struct
{
    Command **commands;
    int numCommands;
} Commands;

#include "prompt.h"
#endif