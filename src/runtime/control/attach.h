#ifndef KVETOSLAVOV_RUNTIME_ATTACH
#define KVETISLAVOV_RUNTIME_ATTACH

#include "command.h"

/**
 * Attach to the selected process.
 * @param args contains the PID of the process 
 * @return 0
 */
int 
runtime_command_attach (struct command_args_t *args);

#endif

