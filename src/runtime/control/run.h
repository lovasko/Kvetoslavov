#ifndef KVETOSLAVOV_RUNTIME_RUN
#define KVETISLAVOV_RUNTIME_RUN

#include "command.h"

/**
 * Start the selected executable.
 * @param args contains the full path 
 * @return 0
 */
int 
runtime_command_run (struct command_args_t *args);

#endif

