#ifndef __COMMAND_H
#define __COMMAND_H

Command *parseCommand(char *command, const char *delimiter);
void freeCommand(Command *cmd);

#endif