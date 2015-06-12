#ifndef KVETOSLAVOV_RUNTIME_GENERAL_GENERAL_H
#define KVETISLAVOV_RUNTIME_GENERAL_GENERAL_H

/**
 * Try to terminate the debuggee and break from main loop.
 * @param argv unused
 * @return 0
 */
int 
runtime_command_exit(struct command_args* cmd_args);

/**
 * Print help message.
 * @param argv unused
 * @return 0
 */
int 
runtime_command_help(struct command_args* cmd_args);

/**
 * Try to terminate the debuggee and break from main loop.
 * @param argv unused
 * @return 0
 */
int 
runtime_command_pid(struct command_args* cmd_args);

/**
 * Select and test correctnes of an executable.
 * This runtime command moves the debugger to the preparing state.
 * @param args used to get the executable name
 * @return 0
 */
int 
runtime_command_select(struct command_args* cmd_args);

/**
 * Print all set variables and lists to the stdout.
 * @param args variables to be printed
 * @return 0
 */
int 
runtime_command_status(struct command_args* cmd_args);

#endif

