#ifndef KVETOSLAVOV_STATE
#define KVETOSLAVOV_STATE

#include "command.h"

#define DEFAULT    1
#define PREPARING  2
#define RUNNING    4
#define ALL_STATES 7

/**
 * Inform user about the debugging phase we are in.
 * @param state the state to be printed
 */
void 
state_print (int state);

/**
 * Check if the state is compatible.
 * @param check 
 * @param command 
 * @return 1 if command is compatible with check, 0 otherwise
 */
int 
state_is_compatible (int check, struct command_t* command);

#endif

