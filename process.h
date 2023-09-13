#ifndef PROCESS_H_
#define PROCESS_H_

void insertProcess(ProcessInfo new_process);

void sigchld_handler();

#endif //PROCESS_H_
