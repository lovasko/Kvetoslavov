#ifndef KVETOSLAVOV_RUNTIME_CONTINUE
#define KVETISLAVOV_RUNTIME_CONTINUE

#include "command.h"

/**
 * Resume the debugee.
 * @param args debugger state 
 * @return 0
 */
int 
runtime_command_continue (struct command_args_t *args);

#endif

