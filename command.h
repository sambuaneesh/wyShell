#ifndef __COMMAND_H
#define __COMMAND_H

Command *parseCommand(char *command);
void freeCommand(Command *cmd);

#endif