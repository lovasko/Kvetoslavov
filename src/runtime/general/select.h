#ifndef KVETOSLAVOV_RUNTIME_SELECT
#define KVETISLAVOV_RUNTIME_SELECT

#include "command.h"

/**
 * Select and test correctnes of an executable.
 * This runtime command moves the debugger to the preparing state.
 * @param args used to get the executable name
 * @return 0
 */
int 
runtime_command_select (struct command_args_t *args);

#endif

