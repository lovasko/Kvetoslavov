#ifndef KVETOSLAVOV_RUNTIME_DETACH
#define KVETISLAVOV_RUNTIME_DETACH

#include "command.h"

/**
 * Detach the debuggee.
 * @param args contains the PID 
 * @return 0
 */
int 
runtime_command_detach (struct command_args_t *args);

#endif

