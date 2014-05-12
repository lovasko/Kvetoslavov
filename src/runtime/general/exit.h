#ifndef KVETOSLAVOV_RUNTIME_EXIT
#define KVETISLAVOV_RUNTIME_EXIT

#include "command.h"

/**
 * Try to terminate the debuggee and break from main loop.
 * @param argv unused
 * @return 0
 */
int 
runtime_command_exit (struct command_args_t *args);

#endif
