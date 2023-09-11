#ifndef TOOLS_H_
#define TOOLS_H_

#include "prompt.h"
#include "command.h"
#include "pastevents.h"

char *initializeGlobalHome();
void warp(Command *cmd);
void executeCommand(char *command, int isBackground);
void proclore(Command *cmd);
void peek(Command *cmd);
void printWyshArt();
void seek(Command *cmd);

void trim(char *str);

int pipeCommand(char *command);

void ioRedirection(char *command);

#endif