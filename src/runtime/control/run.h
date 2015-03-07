#ifndef KVETOSLAVOV_RUNTIME_CONTROL_RUN_H
#define KVETISLAVOV_RUNTIME_CONTROL_RUN_H

#include "command/command.h"

/**
 * Start the selected executable.
 * @param args contains the full path 
 * @return 0
 */
int 
runtime_command_run(struct command_args* cmd_args);

#endif

