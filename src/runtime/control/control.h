#ifndef KVETOSLAVOV_RUNTIME_CONTROL_CONTROL_H
#define KVETOSLAVOV_RUNTIME_CONTROL_CONTROL_H

/**
 * Attach to the selected process.
 * @param args contains the PID of the process 
 * @return 0
 */
int 
runtime_command_attach(struct command_args* cmd_args);

/**
 * Resume the debugee.
 * @param args debugger state 
 * @return 0
 */
int 
runtime_command_continue(struct command_args* cmd_args);

/**
 * Detach the debuggee.
 * @param args contains the PID 
 * @return 0
 */
int 
runtime_command_detach(struct command_args* cmd_args);

/**
 * Start the selected executable.
 * @param args contains the full path 
 * @return 0
 */
int 
runtime_command_run(struct command_args* cmd_args);

/**
 * Kill the debuggee.
 * @param args contains the PID 
 * @return 0
 */
int 
runtime_command_stop(struct command_args* cmd_args);

/**
 * Handle the situation after the debugger was woken up from wait().
 * @param wait_status return value of the wait call
 * @param pid PID of the debuggee
 * @param bp breakpoints
 */
int 
after_wait(int wait_status, pid_t* pid, struct breakpoint** bp);

/**
 * Checks whether process exists.
 * @param pid the PID of the process
 * @return 0 if exists, 1 otherwise
 */
int
runtime_process_exists(pid_t pid);

#endif

