#ifndef KVETOSLAVOV_RUNTIME_GENERAL_GENERAL_H
#define KVETISLAVOV_RUNTIME_GENERAL_GENERAL_H

#include "runtime/context.h"

/**
 * Try to terminate the debuggee and break from main loop.
 * @param ctx unused
 * @return 1
 */
int 
runtime_command_exit(struct context* ctx);

/**
 * Print help message.
 * @param ctx unused
 * @return 0
 */
int 
runtime_command_help(struct context* ctx);

/**
 * Print the PID of the debuggee.
 * @param ctx unused
 * @return 0
 */
int 
runtime_command_pid(struct context* ctx);

/**
 * Select and test correctnes of an executable.
 * This runtime command moves the debugger to the preparing state.
 * @param ctx used to get the executable name
 * @return 0
 */
int 
runtime_command_select(struct context* ctx);

/**
 * Print all set variables and lists to the stdout.
 * @param ctx variables to be printed
 * @return 0
 */
int 
runtime_command_status(struct context* ctx);

#endif

