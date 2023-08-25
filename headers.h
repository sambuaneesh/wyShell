#ifndef HEADERS_H_
#define HEADERS_H_

// default buffer size
#define DEF_SIZE 1024
#define MAX_INPUT_LENGTH 4096

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <limits.h>

extern char *global_home;

#include "prompt.h"
#endif