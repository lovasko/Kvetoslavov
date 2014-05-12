#ifndef KVETOSLAVOV_RUNTIME_EXIT
#define KVETISLAVOV_RUNTIME_EXIT

#include "command.h"

/**
 * Print all set variables and lists to the stdout.
 * @param args variables to be printed
 * @return 0
 */
int 
runtime_command_status (struct command_args_t *args);

#endif

