#ifndef KVETOSLAVOV_RUNTIME_GENERAL_SELECT_H
#define KVETISLAVOV_RUNTIME_GENERAL_SELECT_H

#include "command/command.h"

/**
 * Select and test correctnes of an executable.
 * This runtime command moves the debugger to the preparing state.
 * @param args used to get the executable name
 * @return 0
 */
int 
runtime_command_select(struct command_args* cmd_args);

#endif

