#ifndef KVETOSLAVOV_RUNTIME_CONTROL_DETACH_H
#define KVETISLAVOV_RUNTIME_CONTROL_DETACH_H

#include "command/command.h"

/**
 * Detach the debuggee.
 * @param args contains the PID 
 * @return 0
 */
int 
runtime_command_detach(struct command_args* cmd_args);

#endif

