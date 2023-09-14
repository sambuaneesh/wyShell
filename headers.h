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
#include <termios.h>
#include <signal.h>


#include "colors.h"

extern char *global_home;
extern char *prev_directory;
extern pid_t lastProcessID;

extern char *past_events[MAX_PAST_EVENTS];
extern int num_past_events;

typedef struct
{
    char **argv;
    int argc;
} Command;

typedef struct {
    pid_t pid;
    char command[256];         // Stores the first parameter of the command
    char complete_command[512]; // Stores the complete command including arguments
    int is_background;
    int exit_status; // Store the exit status of terminated processes
    char state[16];
} ProcessInfo;

struct ProcessNode {
    ProcessInfo process_info;
    struct ProcessNode *next;
};

extern struct ProcessNode *process_list_head;

// Function to insert a process into the linked list in lexicographic order

#include "prompt.h"
#include "process.h"

#define printError(fmt, ...) fprintf(stderr, "\033[1;31m" fmt "\033[0m", ##__VA_ARGS__)

#endif