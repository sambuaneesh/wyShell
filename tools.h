#ifndef TOOLS_H_
#define TOOLS_H_

#include "prompt.h"
#include "command.h"
#include "pastevents.h"

char *initializeGlobalHome();
void warp(Command *cmd);
void executeCommand(char *tokens[]);

#endif