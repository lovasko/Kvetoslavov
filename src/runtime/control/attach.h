#ifndef KVETOSLAVOV_RUNTIME_CONTROL_ATTACH_H
#define KVETISLAVOV_RUNTIME_CONTROL_ATTACH_H

#include "command/command.h"

/**
 * Attach to the selected process.
 * @param args contains the PID of the process 
 * @return 0
 */
int 
runtime_command_attach(struct command_args* cmd_args);

#endif

