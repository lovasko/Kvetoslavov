#ifndef KVETOSLAVOV_RUNTIME_CONTROL_STOP_H
#define KVETISLAVOV_RUNTIME_CONTROL_STOP_H

#include "command/command.h"

/**
 * Kill the debuggee.
 * @param args contains the PID 
 * @return 0
 */
int 
runtime_command_stop(struct command_args* cmd_args);

#endif

