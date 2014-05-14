#ifndef KVETOSLAVOV_RUNTIME_STOP
#define KVETISLAVOV_RUNTIME_STOP

#include "command.h"

/**
 * Kill the debuggee.
 * @param args contains the PID 
 * @return 0
 */
int 
runtime_command_stop (struct command_args_t *args);

#endif

