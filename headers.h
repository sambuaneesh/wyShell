#ifndef HEADERS_H_
#define HEADERS_H_

// default buffer size
#define DEF_SIZE 1024
#define MAX_INPUT_LENGTH 4096
#define MAX_PAST_EVENTS 16
#define MAX_PATH_LENGTH 4096
#define PAST_EVENTS_FILE "pastevents.txt"
#define MAX_COMMANDS 10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <pwd.h>
#include <limits.h>
#include <dirent.h>
#include <grp.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netdb.h>


#include "colors.h"

extern char *global_home;
extern char *prev_directory;

extern char *past_events[MAX_PAST_EVENTS];
extern int num_past_events;

typedef struct
{
    char **argv;
    int argc;
} Command;

#include "prompt.h"
#endif