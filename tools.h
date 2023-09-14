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

int ioPipe(char *cmd);

void ioRedirection(char *command);

void iMan(Command *cmd);

void printActivities();

void handleFgBgCommand(Command *cmd);

pid_t deleteLatestProcess();

void copyProcessInfoToLatest(pid_t pid);

#endif