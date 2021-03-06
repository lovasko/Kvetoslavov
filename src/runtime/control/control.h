#ifndef KVETOSLAVOV_RUNTIME_CONTROL_CONTROL_H
#define KVETOSLAVOV_RUNTIME_CONTROL_CONTROL_H

#include <unistd.h>

#include "runtime/context.h"

/**
 * Attach to the selected process.
 * @param args contains the PID of the process 
 * @return 0
 */
int 
runtime_command_attach(struct context* ctx);

/**
 * Resume the debugee.
 * @param args debugger state 
 * @return 0
 */
int 
runtime_command_continue(struct context* ctx);

/**
 * Detach the debuggee.
 * @param args contains the PID 
 * @return 0
 */
int 
runtime_command_detach(struct context* ctx);

/**
 * Start the selected executable.
 * @param args contains the full path 
 * @return 0
 */
int 
runtime_command_run(struct context* ctx);

/**
 * Kill the debuggee.
 * @param args contains the PID 
 * @return 0
 */
int 
runtime_command_stop(struct context* ctx);

/**
 * Handle the situation after the debugger was woken up from wait().
 * @param wait_status return value of the wait call
 * @param pid PID of the debuggee
 * @param bp breakpoints
 */
int 
after_wait(int wait_status, struct context* ctx);

/**
 * Checks whether process exists.
 * @param pid the PID of the process
 * @return 0 if exists, 1 otherwise
 */
int
runtime_process_exists(pid_t pid);

#endif

