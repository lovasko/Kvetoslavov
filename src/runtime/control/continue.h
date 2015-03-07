#ifndef KVETOSLAVOV_RUNTIME_CONTROL_CONTINUE_H
#define KVETISLAVOV_RUNTIME_CONTROL_CONTINUE_H

#include "command/command.h"

/**
 * Resume the debugee.
 * @param args debugger state 
 * @return 0
 */
int 
runtime_command_continue(struct command_args* cmd_args);

#endif

