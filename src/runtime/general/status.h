#ifndef KVETOSLAVOV_RUNTIME_GENERAL_STATUS_H
#define KVETISLAVOV_RUNTIME_GENERAL_STATUS_H

#include "command/command.h"

/**
 * Print all set variables and lists to the stdout.
 * @param args variables to be printed
 * @return 0
 */
int 
runtime_command_status(struct command_args* cmd_args);

#endif

