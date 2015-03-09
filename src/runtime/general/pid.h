#ifndef KVETOSLAVOV_RUNTIME_GENERAL_PID_H
#define KVETISLAVOV_RUNTIME_GENERAL_PID_H

#include "command/command.h"

/**
 * Try to terminate the debuggee and break from main loop.
 * @param argv unused
 * @return 0
 */
int 
runtime_command_pid(struct command_args* cmd_args);

#endif

